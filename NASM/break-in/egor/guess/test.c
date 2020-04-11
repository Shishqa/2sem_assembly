#include<stdio.h>

int main() {

    char str[8] = "password";

    unsigned long hash = 0x6001FE;

    for (size_t i = 0; i < 8; ++i) {

        hash = (33 * hash) ^ str[i];
    }

    printf("\"password\" hash = %lX\n", hash);

    return 0;
}
