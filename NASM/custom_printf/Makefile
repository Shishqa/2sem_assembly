
NASM_FLAGS = -felf64 -gdwarf

all: tests clean

tests: test_put_s test_put_c test_put_u test_put_b test_put_x test_put_o test_put_d

U_BASE = put_u_base.o put_u.o put_s.o put_c.o itoa.o

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

test_put_c: put_c.o test_put_c.o
	ld -o $@ $^

test_put_s: put_s.o put_c.o test_put_s.o
	ld -o $@ $^

%.o: %.asm
	nasm ${NASM_FLAGS} -o $@ $<

clean:
	rm *.o

