#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "c_HashTable.h"

class MurmurHash64B {

    static const unsigned int SEED = 0xFF00FF;

public:

    unsigned long long operator()(const std::string& str_key) const;

};

class StrEqual {

public:

    bool operator()(const std::string& str1, const std::string& str2) const;

};

int main() {

    HashTable<std::string, int, MurmurHash64B, StrEqual> hash_table(0x4000);

    std::ifstream fin("DATA");

    size_t find_request_count = 0;
    fin >> find_request_count;

    std::string str;

    while (fin >> str) {

        ++hash_table[str];

    }

    fin.close();

    const size_t MAX_STR_LEN = 15;

    std::string requests[MAX_STR_LEN] = {
            "a",
            "aa",
            "aaa",
            "aaaa",
            "aaaaa",
            "aaaaaa",
            "aaaaaaa",
            "aaaaaaaa",
            "aaaaaaaaa",
            "aaaaaaaaaa",
            "aaaaaaaaaaa",
            "aaaaaaaaaaaa",
            "aaaaaaaaaaaaa",
            "aaaaaaaaaaaaaa",
            "aaaaaaaaaaaaaaa"
    };

    for (size_t i = 0; i < find_request_count; ++i) {

        hash_table.find(requests[i % MAX_STR_LEN]);

    }

//    std::cout << "Done\n";
//
//    std::ofstream fout("dispersion_change.csv");
//
//    for (auto bucket : hash_table) {
//
//        std::cout << bucket.size() << " ; \n";
//        fout << bucket.size() << " ; \n";
//
//    }
//
//    fout.close();

    return 0;
}


bool StrEqual::operator()(const std::string& str1, const std::string& str2) const {

    const size_t str1_len = str1.length();

    if (str1_len != str2.length()) {
        return false;
    }

    const char* str1_c = str1.c_str();
    const char* str1_end = str1_c + str1_len;
    const char* str2_c = str2.c_str();

    for (; str1_c < str1_end; ++str1_c, ++str2_c) {

        if (*str1_c != *str2_c) {
            return false;
        }

    }

    return true;

//    __asm__ (".intel_syntax noprefix\n"
//             "cld\n"
//             "xor   rax, rax\n"
//             "repe  cmpsb\n"
//             "sete  al\n"
//             ".att_syntax prefix\n"
//    :
//    : "S"(str1.c_str()), "D"(str2.c_str()), "c"(str1_len)
//    : "cc", "rax"
//    );
}

//inline unsigned int mul_m(unsigned int x) {
//    __asm__ (".intel_syntax noprefix\n"
//             "mov   eax, %[x]\n"
//             "imul  eax, eax, 0x336F6D\n"
//             "mov   eax, %[x]\n"
//             "shl   eax, 3\n"
//             "add   %[x], eax\n"
//             "shl   eax, 3\n"
//             "add   %[x], eax\n"
//             "shl   eax, 1\n"
//             "add   %[x], eax\n"
//             "shl   eax, 1\n"
//             "add   %[x], eax\n"
//             ".att_syntax prefix\n"
//            : ""
//            )
//}

#define PROCESS_CHUNK(data, h)           \
__asm__(".intel_syntax noprefix\n"       \
         "mov   ebx, [%[data]]\n"        \
         "add   %[data], 4\n"            \
         "imul  ebx, ebx, M\n"           \
         "mov   ecx, ebx\n"              \
         "shr   ecx, R\n"                \
         "xor   ebx, ecx\n"              \
         "imul  ebx, ebx, M\n"           \
         "imul  %[out], %[out], M\n"     \
         "xor   %[out], ebx\n"           \
         ".att_syntax prefix\n"          \
: "=S" (data), [out] "=a" (h)            \
: [data] "S"(data),  "a"(h)              \
: "ebx", "ecx"                           \
)

//inline unsigned int process_chunk (const unsigned int* data, unsigned int h) {
//
//    __asm__ (".equ R, 24\n"
//             ".equ M, 0x5BD1E995\n"
//    );
//
//    __asm__ (".intel_syntax noprefix\n"
//             "mov   ebx, [%[data]]\n"               // k1 = *data;
//             "imul  ebx, ebx, M\n"                  // k1 *= m;
//             "mov   ecx, ebx\n"
//             "shr   ecx, R\n"
//             "xor   ebx, ecx\n"                     // k1 ^= k1 >> r;
//             "imul  ebx, ebx, M\n"                  // k1 *= m;
//             "imul  %[h], %[h], M\n"                // h1 *= m;
//             "xor   %[h], ebx\n"                    // h1 ^= k1;
//             ".att_syntax prefix\n"
//    : "=S" (data), "=a" (h)
//    : [data] "S"(data), [h] "a"(h)
//    : "ebx", "ecx"
//    );
//}

unsigned long long MurmurHash64B::operator()(const std::string& str_key) const {

    const void* key = str_key.c_str();
    int len = str_key.length();

    __asm__ (".equ R, 24\n"
             ".equ M, 0x5BD1E995\n"
    );

    unsigned int h1 = SEED ^len;
    unsigned int h2 = 0;

    auto data = reinterpret_cast<const unsigned int*>(key);

    while (len >= 8) {
        PROCESS_CHUNK(data, h1);
        PROCESS_CHUNK(data, h2);
        len -= 8;
    }

    if (len >= 4) {
        PROCESS_CHUNK(data, h1);
        len -= 4;
    }

    switch (len) {

        case 3:
            h2 ^= (reinterpret_cast
                    <const unsigned char*>(data)
            )[2] << 16;
        case 2:
            h2 ^= (reinterpret_cast
                    <const unsigned char*>(data)
            )[1] << 8;
        case 1:
            h2 ^= (reinterpret_cast
                    <const unsigned char*>(data)
            )[0];
            __asm__ (".intel_syntax noprefix\n"
                     "imul %[h2], %[h2], M\n"
                     ".att_syntax prefix\n"
            : "=a" (h2)
            : [h2] "a"(h2)
            :
            );
    };

    uint64_t h = 0;

    __asm__ (".intel_syntax noprefix\n"
             "mov   ebx, edx\n"
             "shr   ebx, 18\n"
             "xor   ecx, ebx\n"                 // h1 ^= h2 >> 18;
             "imul  ecx, ecx, M\n"              // h1 *= m;
             "mov   ebx, ecx\n"
             "shr   ebx, 22\n"
             "xor   edx, ebx\n"                 // h2 ^= h1 >> 22;
             "imul  edx, edx, M\n"              // h2 *= m;
             "mov   ebx, edx\n"
             "shr   ebx, 17\n"
             "xor   ecx, ebx\n"                 // h1 ^= h2 >> 17;
             "imul  rax, rcx, M\n"              // h1 *= m;
             "mov   rbx, rax\n"
             "shr   ebx, 19\n"
             "xor   edx, ebx\n"                 // h2 ^= h1 >> 19;
             "imul  edx, edx, M\n"              // h2 *= m;
             "shl   rax, 32\n"
             "or    rax, rdx\n"                 // h = (h << 32) | h2;
             ".att_syntax prefix\n"
    :
    : "c"(h1), "d"(h2)
    : "rbx"
    );
}