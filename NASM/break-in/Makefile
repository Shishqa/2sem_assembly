
all: test_get clean

test_get: test_get.o get_string.o check_password.o
	ld -o $@ $^

%.o: %.asm
	nasm -felf64 -l $@.lst -o $@ $<

clean:
	rm *.o
