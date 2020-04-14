#include "HashTable.h"

#include <unordered_map>

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

const uint32_t MOD = 991;

uint32_t id_hash(const std::string& str);
uint32_t len_hash(const std::string& str);
uint32_t ascii_hash(const std::string& str);
uint32_t simple_hash(const std::string& str);
uint32_t murmur_hash(const std::string& str);

void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out );

class Hasher {
public:
    unsigned long long operator()(const int& key) const {
        return key * 33;
    }
};

class Equal {
public:
    bool operator()(const int& key1, const int& key2) const {
        return key1 == key2;
    }
};

int main() {

    HashTable<int, int, Hasher, Equal> table;

    for (size_t i = 0; i < 100000; ++i) {

        table.insert({static_cast<int>(i), 1});

    }

    for (size_t i = 0; i < 10000; ++i) {

        printf("%d\n", *table.find(i));

    }

    std::cout << table[13] << "\n\n\n\n";

    return 0;
}

uint32_t id_hash(const std::string& str) {

    return str[0];
}

uint32_t len_hash(const std::string& str) {

    return str.length();
}

uint32_t ascii_hash(const std::string& str) {

    uint32_t hash = 0;

    for (auto c : str) {

        hash += c;
    }

    return hash / str.length();
}

uint32_t simple_hash(const std::string& str) {

    uint32_t hash = 0;

    for (auto c : str) {
        hash ^= static_cast<uint32_t>(3001);
        hash += c;
    }

    return hash;
}

uint32_t murmur_hash(const std::string& str) {

    uint32_t hash = 0;

    MurmurHash3_x86_32(str.c_str(), str.length(), 3001, &hash);

    return hash;
}


static inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
return (x << r) | (x >> (32 - r));
}

#define getblock(p, i) (p[i])

static inline uint32_t fmix32 ( uint32_t h )
{
h ^= h >> 16;
h *= 0x85ebca6b;
h ^= h >> 13;
h *= 0xc2b2ae35;
h ^= h >> 16;

return h;
}

void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;
    int i;

    uint32_t h1 = seed;

    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

    for(i = -nblocks; i; i++)
    {
        uint32_t k1 = getblock(blocks,i);

        k1 *= c1;
        k1 = rotl32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rotl32(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3)
    {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
            k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 = fmix32(h1);

    *(uint32_t*)out = h1;
}