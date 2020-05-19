
#include <cstdio>

int fib(const int& n) {

    if (n <= 1) {
        return n;
    }

    return fib(n - 1) + fib(n - 2);
}

int main() {

    fib(45);

    printf("D");

    return 0;
}
