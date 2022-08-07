/*
 * My Prog
 * File:   myprog.c
 * Author: C Michael Sundius
 * Author: msundius@sundius.com
 *
 * Created on July 6, 2022, 7:47 PM
 * Copyright (c) 2022, C Michael Sundius, All rights reserved.
 */
/* 
 * purpose: 
 *  read the elf header and section headers to find the Global Offset Table
 *  From this read and display addresses in memory of libararies that are
 *  loaded into memory.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <errno.h>

#define Bool _Bool
#define true 1
#define false 0

#define MAXFILENAMELEN 512
#define MAXSECTIONNAMELEN  32

int main(int argc, char **argv)
{
	char *filename;
	int filename_len;
	Bool debug;
	Elf64_Ehdr elf_header;
	Elf64_Shdr elf_section_header;
	int nbytes;
	int nsections;
	int sec_off;
	void *got_ptr = NULL;
	int i;
	int fd;

	/* if we passed in a debug flag? */
        if ((argc == 3) && (*argv[2] == 'd')) {
                printf("enable debug\n");
                debug = true;
                argc = 2;
        }

	// Check to make sure user passes two arguments:
        if (argc != 2) {
                printf("Please pass the following three arguments: x, y, z, %d\n", argc);
                exit(1);
        }

	filename = argv[0];
	filename_len = strnlen((const char *)filename, MAXFILENAMELEN);

	if (filename_len >= MAXFILENAMELEN) {
		printf("filename:%s is too long\n", filename);
		exit(1);
	}


	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		printf("Could not open the file\n");
		exit(1);
	}

	nbytes = read(fd, &elf_header, sizeof(Elf64_Ehdr));
	if (nbytes != sizeof(Elf64_Ehdr)) {
		printf("could not read the whole Elf Header\n");
		exit(1);
	}
	
	nsections = elf_header.e_shnum;
	sec_off = elf_header.e_shoff;
	printf("there are %d section headers at offset 0x%x\n", 
		nsections, sec_off);

	nbytes = lseek(fd, sec_off, SEEK_SET);
	if (nbytes != sec_off) {
		printf("failed to seek to section header table: %d, %d\n", nbytes, errno);
		exit(1);
	}

	for(i = 0; i < nsections; i++) {
		nbytes = read(fd, &elf_section_header, sizeof(Elf64_Shdr));
		if (nbytes != sizeof(Elf64_Shdr)) {
			printf("could not read the whole Elf Section Header: %d, %d\n",
				nbytes, errno);
			exit(1);
		}
		if (strnlen(elf_section_header.sh_name, MAXSECTIONNAMELEN) >= MAXSECTIONNAMELEN) {
			printf("something wrong long sction header name\n");
			exit(1);
		}
		printf("section name: %s\n", elf_section_header.sh_name);
		if (strcmp(".got", elf_section_header.sh_name) == 0) {
			got_ptr = elf_section_header.sh_addr;
			printf("found Global Offset Table: 0x%x\n",
				got_ptr);
			break;
		}
	}


	printf("ok now I found my got pointer!!! yay :) 0x%x\n", got_ptr);

		
	exit(0);
}

