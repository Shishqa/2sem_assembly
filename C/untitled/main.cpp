#include <iostream>

void mul_m(unsigned int* h) {

    std::cout << h << " " << *h << std::endl;

    __asm__ __volatile__ (".intel_syntax noprefix\n"
                          "mov      r8, %0\n"
                          "mov      r9d, [r8]\n"
                          "mov      ebx, r9d\n"
                          "shl      ebx, 30\n"
                          "mov      eax, r9d\n"
                          "shl      eax, 28\n"
                          "add      ebx, eax\n"
                          "mov      eax, r9d\n"
                          "mov      ecx, 0xBD1E995\n"
                          "mul      ecx\n"
                          "add      ebx, eax\n"
                          "mov      [r8], ebx\n"
                          ".att_syntax prefix\n"
    :
    : "r" (h)
    : "eax", "ebx", "edx", "ecx", "r8", "r9"
    );

    std::cout << h << " " << *h << std::endl;

}

int main() {

    unsigned int a = 1;

    mul_m(&a);

    std::cout << a << std::endl;

    return 0;
}
