as86 -o bs.o bs.s
bcc -c -ansi bc.c 
bcc -c -ansi bio.c
ld86 -d -o booter bs.o bc.o bio.o /usr/lib/bcc/libc.a
