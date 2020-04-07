#include <stdio.h>
#include <stdlib.h>

int main() {

    char str[7] = "";

    for (char c0 = 'A'; c0 <= 'Z'; ++c0) {
        for (char c1 = 'A'; c1 < 'Z'; ++c1) {
            for (char c2 = 'A'; c2 < 'Z'; ++c2) {
                for (char c3 = 'A'; c3 < 'Z'; ++c3) {
                    for (char c4 = 'A'; c4 < 'Z'; ++c4) {
                        for (char c5 = '0'; c5 < '9'; ++c5) {

                            str[0] = c0;
                            str[1] = c1;
                            str[2] = c2;
                            str[3] = c3;
                            str[4] = c4;
                            str[5] = c5;

                            unsigned long hash = 0;

                            for (int i = 0; i <= 5; ++i) {

                                hash ^= 0x72816354;
                                hash += str[i];
                            }

                            if ((hash & 0xFF) == 0x60) {

                                printf("%llx - %s\n", hash, str);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
