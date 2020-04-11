#include <stdio.h>
#include <stdlib.h>

int main() {

    char str[9] = "aaaaaaty";

    for (char c0 = 'A'; c0 <= 'z'; ++c0) {
        for (char c1 = 'A'; c1 < 'z'; ++c1) {
            for (char c2 = 'a'; c2 < 'z'; ++c2) {
                for (char c3 = 'a'; c3 < 'z'; ++c3) {
                    for (char c4 = 'a'; c4 < 'z'; ++c4) {
                        for (char c5 = 'a'; c5 < 'z'; ++c5) {

                            str[0] = c0;
                            str[1] = c1;
                            str[2] = c2;
                            str[3] = c3;
                            str[4] = c4;
                            str[5] = c5;

                            unsigned long hash = 0x6001FE;

                            for (int i = 0; i < 8; ++i) {

                                hash = (33 * hash) ^ str[i];
                            }

                            if (hash == 0x7ACE3BC034533A61) {

                                printf("%lx - %s\n", hash, str);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
