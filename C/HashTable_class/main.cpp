#include <fstream>
#include <string>

#include "c_HashTable.h"

class MurmurHash64B {

    static const unsigned int DEFAULT_SEED = 0xFF00FF;

public:

    unsigned long long operator()(const std::string& str_key) const;

};

int main() {

    std::ios_base::sync_with_stdio(false);

    HashTable<std::string, int, MurmurHash64B> hash_table(0xBB7);   // = 2999 (primary)

    std::ifstream fin("data_2.txt");

    std::string str;

    while (fin >> str) {

        ++hash_table[str];

    }

    fin.close();

    const size_t MAX_STR_LEN = 15;
    const size_t REQUEST_COUNT = 100000000;

    //runtime - 10s

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

    for (size_t i = 0; i < REQUEST_COUNT; ++i) {

        hash_table.find(requests[i % MAX_STR_LEN]);

    }

    return 0;
}

unsigned long long MurmurHash64B::operator()(const std::string& str_key) const {

        const void* key = str_key.c_str();
        int len = str_key.length();
        unsigned int seed = DEFAULT_SEED;

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