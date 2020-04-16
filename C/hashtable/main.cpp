#include "HashTable.h"

#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

uint64_t MurmurHash64B (const void * key, int len, unsigned int seed);

class StrHasher {

    static const unsigned int BASIC_SEED = 0xAB63F3;

public:
    unsigned long long operator()(const std::string& key) const {
        return MurmurHash64B(key.c_str(), key.length(), BASIC_SEED);
    }
};

class StrEqual {
public:
    bool operator()(const std::string& key1, const std::string& key2) const {
        return key1 == key2;
    }
};

int main() {

    const size_t BUCKET_COUNT = 0x7FFFF;

    HashTable<std::string, size_t, StrHasher, StrEqual> table(BUCKET_COUNT);

    std::ifstream fin("text.txt");

    std::string str;

    while (fin >> str) {

        if (isalpha(str[0])) {
            table[str]++;
        }

    }

    fin.close();

    const size_t NUM_VARIANTS = 10;
    
    std::string arr_requests[NUM_VARIANTS] = {  "a", 
                                                "aa", 
                                                "aaa", 
                                                "aaaa"
                                                "aaaaa", 
                                                "aaaaaa", 
                                                "aaaaaaa", 
                                                "aaaaaaaa", 
                                                "aaaaaaaaa", 
                                                "aaaaaaaaaa" };

    for (size_t i = 0; i < 10000000; ++i) {

        table.find(arr_requests[i % NUM_VARIANTS]);

    }

    return 0;
}

uint64_t MurmurHash64B(const void * key, int len, unsigned int seed) {

    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    unsigned int h1 = seed ^ len;
    unsigned int h2 = 0;

    const unsigned int * data = (const unsigned int *)key;

    while(len >= 8) {

        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;

        unsigned int k2 = *data++;
        k2 *= m; k2 ^= k2 >> r; k2 *= m;
        h2 *= m; h2 ^= k2;
        len -= 4;
    }

    if(len >= 4) {

        unsigned int k1 = *data++;
        k1 *= m; k1 ^= k1 >> r; k1 *= m;
        h1 *= m; h1 ^= k1;
        len -= 4;
    }

    switch(len) {

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

