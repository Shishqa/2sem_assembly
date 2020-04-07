all: macos_security_check clean

macos_security_check: test.o get_string.o check_password.o Murmurhash64B.o
	ld -o $@ $^

test.o: test.asm
	nasm -felf64 test.asm

check_password.o: check_password.asm
	nasm -felf64 check_password.asm

get_string.o: get_string.asm
	nasm -felf64 get_string.asm

Murmurhash64B.o: Murmurhash64B.c
	gcc -Wall -c Murmurhash64B.c

clean:
	rm *.o
