as86 -o bs.o bs.s
bcc -c -ansi bc.c 
ld86 -d -o booter bs.o bc.o /usr/lib/bcc/libc.a
dd if=booter of=vdisk bs=16 count=27 conv=notrunc
dd if=booter of=vdisk bs=512 seek=1 conv=notrunc
