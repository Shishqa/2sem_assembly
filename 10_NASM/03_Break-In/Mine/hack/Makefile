GCC_FLAGS = -Wall -Wextra

BIN := bin
SRC := src

all: $(BIN)/hack clean

$(BIN)/hack: $(SRC)/hack.c
	gcc $(GCC_FLAGS) -o $@ $^

clean:
	rm *.o
