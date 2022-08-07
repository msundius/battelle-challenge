#include <stdio.h>

struct lib_s {
    void *load_addr;
    char *name;
    void *data;
    struct lib_s *next;
};
typedef struct lib_s lib_t;

struct got_s {
    void *number;
    lib_t *lib_ptr;
};
typedef struct got_s got_t;

extern got_t _GLOBAL_OFFSET_TABLE_;

int main(int argc, char **argv)
{

    int debug = 0;

    got_t *gotp = &_GLOBAL_OFFSET_TABLE_;
    lib_t *libp = gotp->lib_ptr;

    if (argc > 1) {
        printf("hello world.\n");
    
        if (*argv[1] == 'd') {
            debug = 1;
        }
    }

    while (libp != NULL) {
        printf("0x%016lx", (long unsigned int)libp->load_addr);
        if (debug) {
            printf(" libname: %s", libp->name);
        }
        printf("\n");
        libp = libp->next;
    }

    return 0;
}
