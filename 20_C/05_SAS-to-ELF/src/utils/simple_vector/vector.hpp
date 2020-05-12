#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename elem_t>
class Vector {
public:
    
    Vector();

    ~Vector();

    explicit Vector(const size_t& capacity);

    Vector(const size_t& capacity, const elem_t& init_val);

    Vector(const Vector& other);

    Vector(Vector&& other);

    Vector& operator=(const Vector& other);

    Vector& operator=(Vector&& other);

    void resize(const size_t& capacity);

    size_t size() const;

    size_t capacity() const;

    const elem_t& operator[](const size_t& idx) const;

    elem_t& operator[](const size_t& idx);

    void push_back(elem_t&& value);

    void push_back(const elem_t& value);

private:

    elem_t* data;
    elem_t* back;

    size_t _capacity;
};

#include "vector.ipp"

#endif //VECTOR_HPP
