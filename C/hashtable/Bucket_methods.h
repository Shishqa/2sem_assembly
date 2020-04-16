//
// Created by shishatskiy on 14.04.2020.
//

#ifndef HASHTABLE_BUCKET_METHODS_H
#define HASHTABLE_BUCKET_METHODS_H

#include <cstdio>
#include "HashTable_methods.h"

template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::Node::Node(const HashTable<Key, T, Hash, KeyEqual>::value_t &value,
                                              const HashTable<Key, T, Hash, KeyEqual>::Node *next) :
        value(value),
        next(next) {
#ifdef DEBUG
    printf("Node %p created from basic constructor\n", this);
#endif
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::Node::Node(const HashTable<Key, T, Hash, KeyEqual>::Node &other) :
        value(other.value),
        next(other.next) {
#ifdef DEBUG
    printf("Node %p created from copying constructor\n", this);
#endif
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::Node::~Node() {
    delete next;
#ifdef DEBUG
    printf("Node %p deleted\n", this);
#endif
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::Bucket::Bucket() :
        Node({{},{}}, nullptr),
        current_size(0) {
#ifdef DEBUG
    printf("Bucket %p created from basic constructor\n", this);
#endif
}


template<class Key, class T, class Hash, class KeyEqual>
HashTable<Key, T, Hash, KeyEqual>::Bucket::Bucket(const HashTable<Key, T, Hash, KeyEqual>::Bucket &other) :
        current_size(other.current_size),
        Node(other.value, other.next) {
#ifdef DEBUG
    printf("Bucket %p created from copying constructor\n", this);
#endif
}


template<class Key, class T, class Hash, class KeyEqual>
const typename HashTable<Key, T, Hash, KeyEqual>::Node *
HashTable<Key, T, Hash, KeyEqual>::Bucket::find_node(const key_t &key) const {

    //TODO: complete with iterator

    const KeyEqual key_eq;

    const Node *it = this;

    while (it) {

        if (key_eq(it->value.key, key)) {
            return it;
        }

        it = it->next;
    }

    return nullptr;
}


template<class Key, class T, class Hash, class KeyEqual>
const typename HashTable<Key, T, Hash, KeyEqual>::mapped_t *
HashTable<Key, T, Hash, KeyEqual>::Bucket::find_data(const key_t &key) const {

    const Node *found_node = find_node(key);

    if (!found_node) {
        return nullptr;
    }

    return &(found_node->value.data);

}


template<class Key, class T, class Hash, class KeyEqual>
typename HashTable<Key, T, Hash, KeyEqual>::value_t &
HashTable<Key, T, Hash, KeyEqual>::Bucket::insert(const value_t &value) {

    auto found_node = const_cast<Node *>(find_node(value.key));

    if (found_node) {

        found_node->value.data = value.data;
        return found_node->value;

    } else {

        Node *this_moved = new Node(this->value, this->next);
        this->next = this_moved;
        this->value = value;

        ++current_size;

        return this->value;
    }

}

template<class Key, class T, class Hash, class KeyEqual>
void HashTable<Key, T, Hash, KeyEqual>::Bucket::erase(const key_t &key) {

    //TODO
}


template<class Key, class T, class Hash, class KeyEqual>
void HashTable<Key, T, Hash, KeyEqual>::Bucket::clear() {

    delete this->next;
    this->next = nullptr;
    this->value = {};

    current_size = 0;

}


template<class Key, class T, class Hash, class KeyEqual>
size_t HashTable<Key, T, Hash, KeyEqual>::Bucket::size() {

    return current_size;

}

#endif //HASHTABLE_BUCKET_METHODS_H
