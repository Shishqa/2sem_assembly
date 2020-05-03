NASM_FLAGS = -felf64 -gdwarf

SRC := src
BIN := bin

all: $(BIN)/test_printf tests clean

MODULES = put_chars.o put_u.o put_d.o put_u_base.o itoa.o

$(BIN)/test_printf: test_printf.o printf.o ${MODULES}
	ld -o $@ $^

tests: $(BIN)/test_put_s $(BIN)/test_put_c $(BIN)/test_put_u $(BIN)/test_put_b $(BIN)/test_put_x $(BIN)/test_put_o $(BIN)/test_put_d

U_BASE = put_u_base.o put_u.o put_chars.o itoa.o

$(BIN)/test_put_d: ${U_BASE} test_put_d.o put_d.o
	ld -o $@ $^

$(BIN)/test_put_x: ${U_BASE} test_put_x.o
	ld -o $@ $^

$(BIN)/test_put_o: ${U_BASE} test_put_o.o
	ld -o $@ $^

$(BIN)/test_put_b: ${U_BASE} test_put_b.o 
	ld -o $@ $^

$(BIN)/test_put_u: ${U_BASE} test_put_u.o 
	ld -o $@ $^

$(BIN)/test_put_c: put_chars.o test_put_c.o
	ld -o $@ $^

$(BIN)/test_put_s: put_chars.o test_put_s.o
	ld -o $@ $^

%.o: $(SRC)/%.asm
	nasm ${NASM_FLAGS} -o $@ $<

clean:
	rm *.o

