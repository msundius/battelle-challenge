/*
* Print the names of ELF sections .
*/
#include <err.h>
#include <fcntl.h>
#include <gelf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <string.h>



void
print_ptype ( size_t pt )
{
    char * s ;
#define C(V) case PT_##V:s=#V; break
    switch ( pt ) {
        C(NULL); C(LOAD); C(DYNAMIC);
        C(INTERP); C(NOTE); C(SHLIB);
        C(PHDR); C(TLS); C(IA_64_UNWIND);
//        C(SUNWBSS); C(SUNWSTACK); C(SUNWDTRACE);
//        C(SUNWCAP);
        default :
         s = " unknown " ;
         break ;
    }
    ( void ) printf ( "\"%s\" " , s );
#undef C
}





int
main ( int argc , char ** argv )
{
    int fd ;
    Elf * e ;
    char * name , *p , pc [4* sizeof ( char )];
    Elf_Scn * scn ;
    Elf64_Shdr * scnhdr ;
    Elf64_Phdr * prhdr_tab ;
    Elf64_Phdr * phdr ;
    Elf_Data * data ;
    GElf_Shdr shdr ;
    size_t n , shstrndx , sz ;
    Elf64_Addr    sh_addr;
    Elf64_Addr    got_addr;
    int           got_num = 0;
    int           sec_num = 0;

    if ( argc != 2)
        errx ( EX_USAGE , " usage : %s file - name " , argv [0]);
    if ( elf_version ( EV_CURRENT ) == EV_NONE )
        errx ( EX_SOFTWARE , " ELF library initialization "
            " failed : %s " , elf_errmsg ( -1));
    if (( fd = open ( argv [1] , O_RDONLY , 0)) < 0)
        err ( EX_NOINPUT , " open \%s\" failed " , argv [1]);
    if (( e = elf_begin ( fd , ELF_C_READ , NULL )) == NULL )
        errx ( EX_SOFTWARE , " elf_begin () failed : %s . " ,
        elf_errmsg ( -1));
    if ( elf_kind ( e ) != ELF_K_ELF )
        errx ( EX_DATAERR , " %s is not an ELF object . " ,
                argv [1]);
    if ( elf_getshdrstrndx (e , & shstrndx ) != 0) 
        errx ( EX_SOFTWARE , " elf_getshdrstrndx () failed : %s . " ,
                elf_errmsg ( -1));
    scn = NULL ;
    sec_num = 0;
    while (( scn = elf_nextscn (e , scn )) != NULL ) {
        if ( gelf_getshdr ( scn , & shdr ) != & shdr )
            errx ( EX_SOFTWARE , " getshdr () failed : %s . " ,
                    elf_errmsg ( -1));
        if (( name = elf_strptr (e , shstrndx , shdr . sh_name )) == NULL )
            errx ( EX_SOFTWARE , " elf_strptr () failed : %s . " ,
                    elf_errmsg ( -1));
        if (( scnhdr = elf64_getshdr(scn)) == NULL )
            errx ( EX_SOFTWARE , " elf64_getshdr () failed : %s . " ,
                    elf_errmsg ( -1));
        
	    sh_addr = scnhdr->sh_addr;
    	if (strcmp(".got", name) == 0) {
    		got_addr = sh_addr;
            got_num = sec_num;
	    }
        ( void ) printf ( " Section %-4.4jd %s (0x%016lx)\n " , ( uintmax_t )
        elf_ndxscn ( scn ) , name, sh_addr);
        sec_num++;
    }
    printf("number of section headers is %d\n", sec_num);

    if (( scn = elf_getscn (e , shstrndx )) == NULL )
        errx ( EX_SOFTWARE , " getscn () failed : %s . " ,
        elf_errmsg ( -1));
    if ( gelf_getshdr ( scn , & shdr ) != & shdr )
        errx ( EX_SOFTWARE , " getshdr ( shstrndx ) failed : %s . " ,
    elf_errmsg ( -1));
    ( void ) printf ( " . shstrab : size =%jd\n" , ( uintmax_t )
                    shdr . sh_size );
    data = NULL ; n = 0;
    while ( n < shdr . sh_size &&
            ( data = elf_getdata ( scn , data )) != NULL ) {
        p = ( char *) data->d_buf ;
        printf ( " %s " , p );
        //while ( p < ( char *) data->d_buf + data->d_size ) {
        //if ( vis ( pc , *p , VIS_WHITE , 0))
        //    printf ( " %s " , pc );
        //    n ++; p ++;
        //    ( void ) putchar ((n%16)?' ':'\n');
        //}
    }

    size_t pn = 0;
    if (elf_getphdrnum (e, &pn ) != 0)
        errx ( EX_DATAERR , " elf_getphdrnum () failed : %s. " ,
            elf_errmsg ( -1));

    printf("number of program headers is %d\n", (int)pn);

    if ((prhdr_tab = elf64_getphdr(e)) == NULL)
        errx ( EX_SOFTWARE , " getphdr () failed : %s. " ,
               elf_errmsg ( -1));

    phdr = &prhdr_tab[got_num];
    # define PRINT_FMT "%-20s0x%jx"
# define PRINT_FIELD(N) \
              do { \
                  ( void ) printf ( PRINT_FMT , #N , ( uintmax_t ) phdr->N ); \
              } while (0)
# define NL() do{(void)printf("\n");} while(0)
        PRINT_FIELD(p_type);
        print_ptype(phdr->p_type); NL();
        PRINT_FIELD(p_offset); NL();
        PRINT_FIELD(p_vaddr); NL();
        PRINT_FIELD(p_paddr); NL();
        PRINT_FIELD(p_filesz); NL();
        PRINT_FIELD(p_memsz); NL();
        PRINT_FIELD(p_flags);
        ( void ) printf ( " [ " );
        if ( phdr->p_flags & PF_X )
            ( void ) printf ( " execute " );
        if ( phdr->p_flags & PF_R )
            ( void ) printf ( " read " );
        if ( phdr->p_flags & PF_W )
            ( void ) printf ( " write " );
        printf ( " ] " ); NL ();
        PRINT_FIELD(p_align); NL();



    ( void ) putchar('\n');
    ( void ) elf_end ( e );
    ( void ) close ( fd );

    printf("hello got_addr is now: 0x%016lx\n", got_addr);
    exit ( EX_OK );
}
