
#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#define DEBUG 1

#include <cstdio>
#include <utility>
#include <stdexcept>

#define Element std::pair
#define KEY first
#define DATA second

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
class HashTable {
public:

    HashTable();

    HashTable(const HashTable &table);

    HashTable(Element<key_t, data_t> *data, size_t n);

    ~HashTable();

    data_t &operator[](key_t key);

    size_t size();

private:

    struct ChainNode {
        size_t size;
        Element<key_t, data_t> elem;
        ChainNode *next;

        ChainNode(Element<key_t, data_t> elem, ChainNode const *next = nullptr);

        ChainNode(const ChainNode &node);

        ~ChainNode();

        void insert(Element<key_t, data_t>);
    };

    ChainNode *hash_table;
    size_t curr_size;
};

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode::ChainNode(const std::pair<key_t, data_t> elem,
                                                                          HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode const *next)

                                                                          : elem(elem), next(next){



}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::HashTable()
        : curr_size(0) {

    hash_table = new ChainNode *[MAX_HASH_VALUE];

    if (!hash_table) {
        throw std::overflow_error("Cannot allocate memory for hashtable\n");
    }

#ifdef DEBUG
    printf("HashTable %p init by empty constructor\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::HashTable(const HashTable &table)
        : curr_size(table.curr_size) {

    hash_table = new ChainNode *[MAX_HASH_VALUE];

    if (!hash_table) {
        throw std::overflow_error("Cannot allocate memory for hashtable\n");
    }

    for (__uint32_t i = 0; i < MAX_HASH_VALUE; ++i) {
        hash_table[i] = new ChainNode(*(table.hash_table[i]));
    }

#ifdef DEBUG
    printf("HashTable %p constructed from another table\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::HashTable(Element<key_t, data_t> *data, size_t n)
        : curr_size(n) {

    hash_table = new ChainNode *[MAX_HASH_VALUE];

    if (!hash_table) {
        throw std::overflow_error("Cannot allocate memory for hashtable\n");
    }

    for (size_t i = 0; i < n; ++i) {
        hash_table[hash_func(data[i].KEY)].insert(data[i]);
    }

#ifdef DEBUG
    printf("HashTable %p constructed from array of keys\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(key_t), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::~HashTable() {

    delete hash_table;

#ifdef DEBUG
    printf("Deleted HashTable %p\n", this);
#endif
}

#endif //HASHTABLE_HASHTABLE_H
