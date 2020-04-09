#include "HashTable.h"

#include <cstdio>
#include <string>

__uint32_t simple_hash(const std::string& str);

int main() {

    HashTable<std::string, int, simple_hash, 0xFFF> table;

    ++table["vasya"];

    table.traverse();

    return 0;
}

__uint32_t simple_hash(const std::string& str) {

    __uint32_t hash = 0;

    for (auto c : str) {
        hash ^= static_cast<__uint32_t>(0x72896521);
        hash += c;
    }

    return hash & static_cast<__uint32_t>(0xFFF);
}