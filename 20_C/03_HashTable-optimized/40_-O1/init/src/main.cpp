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

    HashTable<std::string, int, MurmurHash64B, StrEqual> hash_table(0x3FFF);

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
//
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

    for (size_t i = 0; i < str1_len; ++i) {

        if (str1[i] != str2[i]) {

            return false;

        }

    }

    return true;

}

unsigned long long MurmurHash64B::operator()(const std::string& str_key) const {

    const void* key = str_key.c_str();
    int len = str_key.length();

    const int r = 24;
    const unsigned int m = 0x5BD1E995;

    unsigned int h1 = SEED ^len;
    unsigned int h2 = 0;

    auto data = reinterpret_cast<const unsigned int*>(key);

    while (len >= 8) {

        unsigned int k1 = *data++;
        k1*=m;
        k1 ^= k1 >> r;
        k1*=m;
        h1*=m;
        h1 ^= k1;
        len -= 4;

        unsigned int k2 = *data++;
        k2*=m;
        k2 ^= k2 >> r;
        k2*=m;
        h2*=m;
        h2 ^= k2;
        len -= 4;
    }

    if (len >= 4) {

        unsigned int k1 = *data++;
        k1*=m;
        k1 ^= k1 >> r;
        k1*=m;
        h1*=m;
        h1 ^= k1;
        len -= 4;
    }

    switch (len) {

        case 3:
            h2 ^= (reinterpret_cast<const unsigned char*>(data))[2] << 16;
        case 2:
            h2 ^= (reinterpret_cast<const unsigned char*>(data))[1] << 8;
        case 1:
            h2 ^= (reinterpret_cast<const unsigned char*>(data))[0];
            h2*=m;
    };

    h1 ^= h2 >> 18;
    h1*=m;

    h2 ^= h1 >> 22;
    h2*=m;

    h1 ^= h2 >> 17;
    h1*=m;

    h2 ^= h1 >> 19;
    h2*=m;

    uint64_t h = h1;

    h = (h << 32) | h2;

    return h;
}