
#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#define DEBUG 1

#include <cstdio>
#include <utility>
#include <stdexcept>

#define Element std::pair
#define KEY first
#define DATA second

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
class HashTable {
public:

    HashTable();

    HashTable(const HashTable &table);

    HashTable(Element<key_t, data_t> *data, size_t n);

    ~HashTable();

    data_t &operator[](key_t key);

    size_t size();

    void traverse();

private:

    struct ChainNode {
        size_t size;
        Element<key_t, data_t> elem;
        ChainNode *next;

        ChainNode() = delete;

        explicit ChainNode(Element<key_t, data_t> elem, ChainNode *next = nullptr);

        ChainNode(const ChainNode &node);

        ~ChainNode();

        ChainNode *search(const key_t &key);
    };

    ChainNode **hash_table;
    size_t curr_size;
};

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode::ChainNode(const std::pair<key_t, data_t> elem,
                                                                          HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode *next)
        : elem(elem), next(next), size(1) {

#ifdef DEBUG
    printf("ChainNode %p init by standard constructor\n", this);
#endif
}


template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode::ChainNode(const ChainNode &node)
        : elem(node.elem), size(node.size) {

    if (node.next) {
        next = new ChainNode(*node.next);
    }

#ifdef DEBUG
    printf("ChainNode %p init by copying ChainNode %p\n", this, &node);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode::~ChainNode() {

    delete next;

#ifdef DEBUG
    printf("ChainNode %p deleted\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
typename HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode *
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::ChainNode::search(const key_t &key) {

    if (elem.KEY == key) {
        return this;
    } else {
        if (next) {
            return next->search(key);
        }
        return nullptr;
    }
}


template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::HashTable()
        : curr_size(0) {

    hash_table = new ChainNode *[MAX_HASH_VALUE + 1];

    if (!hash_table) {
        throw std::overflow_error("Cannot allocate memory for hashtable\n");
    }

#ifdef DEBUG
    printf("HashTable %p init by empty constructor\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::HashTable(const HashTable &table)
        : curr_size(table.curr_size) {

    hash_table = new ChainNode *[MAX_HASH_VALUE];

    if (!hash_table) {
        throw std::overflow_error("Cannot allocate memory for hashtable\n");
    }

    for (__uint32_t i = 0; i <= MAX_HASH_VALUE; ++i) {
        hash_table[i] = new ChainNode(*(table.hash_table[i]));
    }

#ifdef DEBUG
    printf("HashTable %p constructed from another table\n", this);
#endif
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
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

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::~HashTable() {

    delete[] hash_table;

#ifdef DEBUG
    printf("Deleted HashTable %p\n", this);
#endif
}


template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
data_t &HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::operator[](key_t key) {

    __uint32_t key_hash = hash_func(key);

    ChainNode *search_result = (hash_table[key_hash] ? hash_table[key_hash]->search(key) : nullptr);

    if (!search_result) {
        search_result = new ChainNode({key, {}}, hash_table[key_hash]);
        search_result->size = hash_table[key_hash]->size + 1;

        hash_table[key_hash] = search_result;
        ++curr_size;
    }

    return search_result->elem.DATA;
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
size_t HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::size() {

    return curr_size;
}

template<typename key_t, typename data_t, __uint32_t (*hash_func)(const key_t &), __uint32_t MAX_HASH_VALUE>
void HashTable<key_t, data_t, hash_func, MAX_HASH_VALUE>::traverse() {

    ChainNode *curr_chain = nullptr;

    for (__uint32_t i = 0; i <= MAX_HASH_VALUE; ++i) {

        if (hash_table[i]) {
            printf("%lu ;", hash_table[i]->size);
        } else {
            printf("0; ");
        }

    }

}

#endif //HASHTABLE_HASHTABLE_H
