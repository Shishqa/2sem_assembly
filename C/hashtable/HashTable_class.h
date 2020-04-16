//
// Created by shishatskiy on 14.04.2020.
//

#ifndef HASHTABLE_HASHTABLE_CLASS_H
#define HASHTABLE_HASHTABLE_CLASS_H

#include <cstdio>
#include <utility>
#include <stdexcept>

template <class Key,
          class T,
          class Hash = std::hash<Key>,
          class KeyEqual = std::equal_to<Key>>
class HashTable {
protected:
    //Default hashtable size
    static const size_t DEFAULT_BUCKET_COUNT = 0x7FFFF;     // == 524287 (primary)
    const size_t BUCKET_COUNT;

    //Types
    typedef Key key_t;
    typedef T mapped_t;

    // Value
    struct Value {
        key_t key;
        T data;

        Value() = delete;
        Value(const key_t& key, const T& data) :
                key(key), data(data) {}

        Value(const Value& other) = default;
        ~Value() = default;

        Value& operator=(const Value& right) {
            if (this == &right) {
                return *this;
            }
            key = right.key;
            data = right.data;
            return *this;
        }
    };
    typedef Value value_t;

    // Iterator definition
    template <typename ValueType>
    class Iterator;

    typedef Iterator<value_t> iterator;
    typedef Iterator<const value_t> const_iterator;

public:
    //Constructors
    explicit HashTable(const size_t bucket_count = DEFAULT_BUCKET_COUNT);

    HashTable(std::initializer_list<value_t> init,
              const size_t& bucket_count);

    HashTable(const HashTable& other);

    //Destructor
    ~HashTable();

    //Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;

    iterator begin(const size_t& bucket_idx) noexcept;
    const_iterator begin(const size_t& bucket_idx) const noexcept;

    iterator end(const size_t& bucket_idx) noexcept;
    const_iterator end(const size_t& bucket_idx) const noexcept;

    //Lookup
    mapped_t& operator[](const key_t &key);
    const mapped_t& operator[](const key_t &key) const;

    mapped_t* find(const key_t& key);
    const mapped_t* find(const key_t& key) const;

    bool count(const key_t& key) const;


    //Modification
    value_t& insert(const value_t& value);

    void erase(const key_t& key);

    void clear();

    //Info
    size_t size() const ;

protected:

    template <typename ValueType>
    class Iterator: public std::iterator<std::forward_iterator_tag, ValueType> {
    private:
        Iterator(ValueType* ptr);

    public:
        Iterator(const Iterator& iter);

        typename Iterator::reference operator*() const;
        Iterator& operator++();

    private:
        ValueType* ptr;
    };

    struct Node {
        value_t value;
        const Node* next;

        explicit Node(const value_t& value, const Node* next = nullptr);
        Node(const Node& other);
        ~Node();
    };

    struct Bucket : public Node {

        Bucket();
        Bucket(const Bucket& other);
        ~Bucket() = default;

        const mapped_t* find_data(const key_t& key) const;

        const Node* find_node(const key_t& key) const;

        value_t& insert(const value_t& value);

        void erase(const key_t& key);

        void clear();

        size_t size();

        class Iterator : public std::iterator<std::forward_iterator_tag, ValueType> {
        private:
            Iterator(ValueType* ptr);
        public:
            Iterator(const Iterator& it);

            bool operator!=(Iterator const& other) const;
            bool operator==(Iterator const& other) const;
            typename Iterator::reference operator*() const;
            Iterator& operator++();
        private:
            ValueType* p;
        }

    private:
        size_t current_size;
    };

    Bucket* hash_table;
    size_t current_size;
};


#endif //HASHTABLE_HASHTABLE_CLASS_H
