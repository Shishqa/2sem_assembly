NASM_FLAGS = -felf64 -strip-all
GCC_FLAGS  = -Wall -Wextra

SRC := src/$(target)
BIN := bin

all: $(target) clean

$(target): $(BIN)/$(target)_check   

MODULES = main.o check_password.o get_string.o Murmurhash64B.o

$(BIN)/$(target)_check: $(MODULES)
	ld -o $@ $^

$(BIN)/linux_check: $(MODULES)
	ld -o $@ $^

main.o: $(SRC)/main.asm
	nasm $(NASM_FLAGS) -o $@ $^

check_password.o: $(SRC)/check_password.asm
	nasm $(NASM_FLAGS) -o $@ $^ 

get_string.o: $(SRC)/get_string.asm
	nasm $(NASM_FLAGS) -o $@ $^

Murmurhash64B.o: $(SRC)/Murmurhash64B.c
	gcc $(GCC_FLAGS) -c $^ -o $@

clean:
	rm *.o
