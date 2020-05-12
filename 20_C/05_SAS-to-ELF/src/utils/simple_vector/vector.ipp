#include <iostream>

template <typename elem_t>
Vector<elem_t>::Vector() :
        data(nullptr),
        back(nullptr),
        _capacity(0) {
    std::cout << "constructed empty vector of capacity 0\n";
}

template <typename elem_t>
Vector<elem_t>::~Vector() {

    std::cout << "destructing " << this << "\n";

    if (!data) {
        return;
    }

    auto size = static_cast<size_t>(back - data);

    for (size_t i = 0; i < size; ++i) {
        data[i].~elem_t();
        std::cout << "elem " << data + i << "destructed\n";
    }   

    ::operator delete(data);

    std::cout << "vector deleted\n";
}

template <typename elem_t>
Vector<elem_t>::Vector(const size_t& capacity) :
        data(nullptr),
        back(nullptr),
        _capacity(capacity) {

    data = reinterpret_cast<elem_t*>(::operator new(_capacity * sizeof(elem_t)));
    back = data;

    std::cout << "constructed empty vector of capacity " << _capacity << "\n";
}

template <typename elem_t>
Vector<elem_t>::Vector(const size_t& capacity, const elem_t& init_val) :
        data(nullptr),
        back(nullptr),
        _capacity(capacity) {

    data = reinterpret_cast<elem_t*>(::operator new(_capacity * sizeof(elem_t)));
    back = data + _capacity;

    for (size_t i = 0; i < _capacity; ++i) {
        new(data + i) elem_t(init_val);
    }

    std::cout << "constructed vector of capacity " << _capacity << " filled with value\n";
}

template <typename elem_t>
Vector<elem_t>::Vector(const Vector& other) :
        data(nullptr),
        back(nullptr),
        _capacity(other._capacity) {

    data = reinterpret_cast<elem_t*>(::operator new(_capacity * sizeof(elem_t)));
    back = data + (other.back - other.data);

    for (size_t i = 0; i < _capacity; ++i) {
        new(data + i) elem_t(other.data[i]);
    }

    std::cout << "vector is copied\n";
}

template <typename elem_t>
Vector<elem_t>::Vector(Vector&& other) :
        data(other.data),
        back(other.back),
        _capacity(other._capacity) {

    other.data = nullptr;

    std::cout << "moved vector\n";
}

template <typename elem_t> 
Vector<elem_t>& Vector<elem_t>::operator=(const Vector& other) {

    if (this == &other) {
        return *this;
    }

    ::operator delete(data);

    _capacity = other._capacity;
    data = reinterpret_cast<elem_t*>(::operator new(_capacity * sizeof(elem_t)));
    back = data + (other.back - other.data);

    for (size_t i = 0; i < _capacity; ++i) {
        new(data + i) elem_t(other.data[i]);
    }

    std::cout << "vector copied and assigned\n";

    return *this;
}

template <typename elem_t>
Vector<elem_t>& Vector<elem_t>::operator=(Vector&& other) {

    _capacity = other._capacity;

    ::operator delete(data);
    data = other.data;
    back = other.back;

    other.data = nullptr;

    std::cout << "vector move assigned\n";
        
    return *this;
}

template <typename elem_t>
void Vector<elem_t>::resize(const size_t& new_capacity) {

    if (new_capacity == _capacity) {
        return;
    }

    elem_t* new_data = reinterpret_cast<elem_t*>(::operator new(new_capacity * sizeof(elem_t)));

    for (size_t i = 0; i < std::min(new_capacity, _capacity); ++i) {
        new(new_data + i) elem_t(std::move(data[i]));
    }

    _capacity = new_capacity;

    size_t size = back - data;

    ::operator delete(data);
    data = new_data;
    back = (size < _capacity ? data + size : 
                               data + _capacity);

    std::cout << "vector resized to size " << new_capacity << "\n";
}

template <typename elem_t>
size_t Vector<elem_t>::size() const {
    return (data ? back - data : 0);
}

template <typename elem_t>
size_t Vector<elem_t>::capacity() const {
    return _capacity;
}

template <typename elem_t>
const elem_t& Vector<elem_t>::operator[](const size_t& idx) const {
    return data[idx];
}

template <typename elem_t>
elem_t& Vector<elem_t>::operator[](const size_t& idx) {
    return const_cast<elem_t&>(const_cast<const Vector*>(this)->operator[](idx));
}

template <typename elem_t>
void Vector<elem_t>::push_back(const elem_t& value) {

    if (!data) {
        resize(1);
    } else if (static_cast<size_t>(back - data) == _capacity) {
        resize(_capacity * 2);
    }

    new (back++) elem_t(value);
}

template <typename elem_t>
void Vector<elem_t>::push_back(elem_t&& value) {

    if (!data) {
        resize(1);
    } else if (static_cast<size_t>(back - data) == _capacity) {
        resize(_capacity * 2);
    }

    new (back++) elem_t(std::move(value));
}
