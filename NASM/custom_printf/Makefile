
NASM_FLAGS = -felf64 -gdwarf

all: test_printf tests clean

MODULES = put_chars.o put_u.o put_d.o put_u_base.o itoa.o

test_printf: test_printf.o printf.o ${MODULES}
	ld -o $@ $^

tests: test_put_s test_put_c test_put_u test_put_b test_put_x test_put_o test_put_d

U_BASE = put_u_base.o put_u.o put_chars.o itoa.o

test_put_d: ${U_BASE} test_put_d.o put_d.o
	ld -o $@ $^

test_put_x: ${U_BASE} test_put_x.o
	ld -o $@ $^

test_put_o: ${U_BASE} test_put_o.o
	ld -o $@ $^

test_put_b: ${U_BASE} test_put_b.o 
	ld -o $@ $^

test_put_u: ${U_BASE} test_put_u.o 
	ld -o $@ $^

test_put_c: put_chars.o test_put_c.o
	ld -o $@ $^

test_put_s: put_chars.o test_put_s.o
	ld -o $@ $^

%.o: %.asm
	nasm ${NASM_FLAGS} -l $@.lst -o $@ $<

clean:
	rm *.o

