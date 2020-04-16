//
// Created by shishatskiy on 14.04.2020.
//

#ifndef HASHTABLE_HASHTABLE_METHODS_H
#define HASHTABLE_HASHTABLE_METHODS_H

#include "HashTable_class.h"

template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::HashTable(const size_t bucket_count) :
        BUCKET_COUNT(bucket_count),
        current_size(0) {

    hash_table = new Bucket[BUCKET_COUNT];
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::HashTable(const HashTable &other) :
        BUCKET_COUNT(other.BUCKET_COUNT),
        current_size(other.current_size) {

    hash_table = new Bucket[BUCKET_COUNT];

    for (size_t i = 0; i < BUCKET_COUNT; ++i) {

        hash_table[i] = new Bucket(other.hash_table[i]);

    }
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::~HashTable() {

    delete[] hash_table;
}


template<class Key, class T, class Hash, class KeyEqual>
const typename HashTable<Key, T, Hash, KeyEqual>::mapped_t &
HashTable<Key, T, Hash, KeyEqual>::operator[](const key_t &key) const {

    const mapped_t *found = find(key);

    if (!found) {

        throw std::runtime_error("bad using of operator[] in const class");

    } else {

        return *found;

    }
}


template<class Key, class T, class Hash, class KeyEqual>
typename HashTable<Key, T, Hash, KeyEqual>::mapped_t &
HashTable<Key, T, Hash, KeyEqual>::operator[](const key_t &key) {

    mapped_t *found = find(key);

    if (!found) {

        value_t &inserted = insert({key, {}});
        return inserted.data;

    } else {

        return *found;

    }

}


template<class Key, class T, class Hash, class KeyEqual>
const typename HashTable<Key, T, Hash, KeyEqual>::mapped_t *
HashTable<Key, T, Hash, KeyEqual>::find(const key_t &key) const {

    const Hash hasher;

    return hash_table[hasher(key) % BUCKET_COUNT].find_data(key);

}


template<class Key, class T, class Hash, class KeyEqual>
typename HashTable<Key, T, Hash, KeyEqual>::mapped_t *
HashTable<Key, T, Hash, KeyEqual>::find(const key_t &key) {

    const mapped_t *data = static_cast<const HashTable<Key, T, Hash, KeyEqual> &>(*this).find(key);
    return const_cast<typename HashTable<Key, T, Hash, KeyEqual>::mapped_t *>(data);

}


template<class Key, class T, class Hash, class KeyEqual>
bool HashTable<Key, T, Hash, KeyEqual>::count(const key_t &key) const {

    return !(find(key) == nullptr);

}


template<class Key, class T, class Hash, class KeyEqual>
typename HashTable<Key, T, Hash, KeyEqual>::value_t &
HashTable<Key, T, Hash, KeyEqual>::insert(const value_t &value) {

    const Hash hasher;

    unsigned long long idx = hasher(value.key) % BUCKET_COUNT;
    size_t old_size = hash_table[idx].size();

    value_t &val = hash_table[idx].insert(value);

    (old_size < hash_table[idx].size() ? ++current_size : current_size);

    return val;
}


template<class Key, class T, class Hash, class KeyEqual>
void HashTable<Key, T, Hash, KeyEqual>::erase(const key_t &key) {

    const Hash hasher;

    hash_table[hasher(key) % BUCKET_COUNT].erase(key);

}


template<class Key, class T, class Hash, class KeyEqual>
void HashTable<Key, T, Hash, KeyEqual>::clear() {

    for (size_t i = 0; i < BUCKET_COUNT; ++i) {
        hash_table[i].clear();
    }
    current_size = 0;

}


#endif //HASHTABLE_HASHTABLE_METHODS_H
