#include <fstream>
#include <iostream>
#include <string>
#include <nmmintrin.h>

#include "c_HashTable.h"

class HashCRC32 {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashID {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashLEN {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashASCIISUM {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashJUSTXOR {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashXORROT {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class HashMURMUR {
public:

    uint64_t operator()(const std::string& str_key) const;

};

class StrEqual {
public:

    inline bool operator()(const std::string& str1, const std::string& str2) const __attribute__((always_inline));

};


int main() {

    const size_t HASHTABLE_SIZE = 6611; // primary

    HashTable<std::string, int, HashCRC32, StrEqual> hash_table(HASHTABLE_SIZE);

    std::ifstream fin("DATA");

    size_t find_request_count = 0;
    fin >> find_request_count;

    std::string str;

    while (fin >> str) {

        ++hash_table[str];

    }

    fin.close();

//    const size_t MAX_STR_LEN = 15;
//
//    std::string requests[MAX_STR_LEN] = {
//            "a",
//            "aa",
//            "aaa",
//            "aaaa",
//            "aaaaa",
//            "aaaaaa",
//            "aaaaaaa",
//            "aaaaaaaa",
//            "aaaaaaaaa",
//            "aaaaaaaaaa",
//            "aaaaaaaaaaa",
//            "aaaaaaaaaaaa",
//            "aaaaaaaaaaaaa",
//            "aaaaaaaaaaaaaa",
//            "aaaaaaaaaaaaaaa"
//    };
//
//    for (size_t i = 0; i < find_request_count; ++i) {
//
//        hash_table.find(requests[i % MAX_STR_LEN]);
//
//    }

    size_t dispersion[HASHTABLE_SIZE + 1] = {};
    size_t size[HASHTABLE_SIZE + 1] = {};

    size_t i = 0;
    for (auto bucket : hash_table) {
        size[i++] = bucket.size();
        ++dispersion[bucket.size()];
    }

    std::ofstream fout("dispersions/crc32.csv");

    for (size_t i = 0; i < HASHTABLE_SIZE + 1; ++i) {
        fout << dispersion[i] << "; " << size[i] << "\n";
    }

    fout.close();

    return 0;
}


bool StrEqual::operator()(const std::string& str1, const std::string& str2) const {

    const char* str1_c = str1.c_str();
    const char* str2_c = str2.c_str();
    int check = 0;

    __asm__ (".intel_syntax noprefix\n"
             "sub       rax, rdx\n"
             "sub       rdx, 16\n"
             ".strloop:\n"
             "add       rdx, 16\n"
             "movdqu    xmm0, [rdx]\n"
             "pcmpistri xmm0, [rdx+rax], 0b00011000\n"
             "ja        .strloop\n"
             "jc        .not_eq\n"
             "mov       rax, 1\n"
             "jmp       .quit\n"
             ".not_eq:\n"
             "xor       rax, rax\n"
             ".quit:\n"
             ".att_syntax prefix\n"
            : "=a" (check)
            : "a" (str1_c), "d" (str2_c)
            : "xmm0", "rcx"
            );

            return check;

//    const size_t str1_len = str1.length();
//
//    if (str1_len != str2.length()) {
//        return false;
//    }
//
//    const char* str1_c = str1.c_str();
//    const char* str2_c = str2.c_str();
//    const char* str1_end = str1_c + str1_len;
//
//    for (;str1_c < str1_end; ++str1_c, ++str2_c) {
//        if (*str1_c != *str2_c) {
//            return false;
//        }
//    }
//
//    return true;
}

uint64_t HashID::operator()(const std::string &str_key) const {

    return str_key[0];
}

uint64_t HashLEN::operator()(const std::string &str_key) const {

    return str_key.length();
}

uint64_t HashASCIISUM::operator()(const std::string &str_key) const {

    size_t len = 0;
    uint64_t hash = 0;

    for (auto c : str_key) {
        hash += c;
        ++len;
    }

    return hash / len;
}

uint64_t HashJUSTXOR::operator()(const std::string &str_key) const {

    const uint64_t mod = 0x63D281FF;

    uint64_t hash = 0;

    for (auto c : str_key) {
        hash = (hash + c) ^ mod;
    }

    return hash;
}

uint64_t HashXORROT::operator()(const std::string &str_key) const {

    uint64_t highest_bit_mask = 1 << 63;
    uint64_t hash = 0;

    for (auto c : str_key) {
        hash ^= c;
        hash = ((hash & highest_bit_mask) >> 63) | (hash << 1);
    }

    return hash;
}

uint64_t HashMURMUR::operator()(const std::string &str_key) const {

    size_t len = str_key.length();

    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h1 = 0xFF13FF ^ len;
    unsigned int h2 = 0;

    const unsigned int * data = (const unsigned int *) str_key.c_str();

    while(len >= 8)
    {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;

        unsigned int k2 = *data++;
        k2 *= m; k2 ^= k2 >> r; k2 *= m;
        h2 *= m; h2 ^= k2;
        len -= 4;
    }

    if(len >= 4)
    {
        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;
    }

    switch(len)
    {
        case 3: h2 ^= ((unsigned char*)data)[2] << 16;
        case 2: h2 ^= ((unsigned char*)data)[1] << 8;
        case 1: h2 ^= ((unsigned char*)data)[0];
            h2 *= m;
    };

    h1 ^= h2 >> 18; h1 *= m;
    h2 ^= h1 >> 22; h2 *= m;
    h1 ^= h2 >> 17; h1 *= m;
    h2 ^= h1 >> 19; h2 *= m;

    uint64_t h = h1;

    h = (h << 32) | h2;

    return h;

}

uint64_t HashCRC32::operator()(const std::string& str_key) const {

    const void* key = str_key.c_str();
    int len = str_key.length();

    __asm__ (".intel_syntax noprefix\n"
             "xor       rax, rax\n"
             ".loop_crc32_qword:\n"
             "cmp       rcx, 8\n"
             "jb        .crc32_dword\n"
             "mov       rdi, [rsi]\n"
             "crc32     rax, rdi\n"
             "add       rsi, 8\n"
             "sub       rcx, 8\n"
             "jmp       .loop_crc32_qword\n"
             ".crc32_dword:\n"
             "cmp       rcx, 4\n"
             "jb        .crc32_word\n"
             "mov       edi, [rsi]\n"
             "crc32d    eax, edi\n"
             "add       rsi, 4\n"
             "sub       rcx, 4\n"
             ".crc32_word:\n"
             "cmp       rcx, 2\n"
             "jb        .crc32_byte\n"
             "mov       di, [rsi]\n"
             "crc32w    eax, di\n"
             "add       rsi, 2\n"
             "sub       rcx, 2\n"
             ".crc32_byte:\n"
             "cmp       rcx, 0\n"
             "jz        .exit\n"
             "mov       dil, [rsi]\n"
             "crc32b    rax, dil\n"
             ".exit:\n"
             ".att_syntax prefix\n"
            :
            : "S" (key), "c" (len)
            : "rax"
            );

//    while (len >= 8) {
//        hash = _mm_crc32_u64(hash, *reinterpret_cast<const uint64_t*>(key));
//        len -= 8;
//        key += sizeof(uint64_t);
//    }

//    if (len >= 4) {
//        hash = _mm_crc32_u32(hash, *reinterpret_cast<const uint32_t*>(key));
//        len -= 4;
//        key += sizeof(uint32_t);
//    }
//
//    if (len >= 2) {
//        hash = _mm_crc32_u16(hash, *reinterpret_cast<const uint16_t*>(key));
//        len -= 2;
//        key += sizeof(uint16_t);
//    }
//
//    if (len) {
//        hash = _mm_crc32_u8(hash, *reinterpret_cast<const uint8_t*>(key));
//    }
//
//    return hash;
}