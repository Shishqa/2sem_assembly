//
// Created by shishqa on 4/30/20.
//

#define NDEBUG

#ifdef NDEBUG
    #define $DBG if(0) 
#else 
    #define $DBG if(1)
#endif

#include "Image.h"
#include <cstdio>
#include <cassert>

Image::Image(const Image& other) :
        _width(other._width),
        _height(other._height),
        _buf_size(other._buf_size),
        _info_size(other._info_size),
        _pixbuf_offset(other._pixbuf_offset) {

    buf = new(std::align_val_t{ALIGN}) char[_buf_size];

    for (size_t i = 0; i < _buf_size; ++i) {
        buf[i] = other.buf[i];
    }

    pixbuf = new uint32_t*[_height];

    uint32_t* pix_ptr = reinterpret_cast<uint32_t*>(buf + _pixbuf_offset);

    assert(reinterpret_cast<uintptr_t>(pix_ptr) % ALIGN == 0);

    for (size_t y = 0; y < _height; ++y) {
        pixbuf[y] = pix_ptr;
        pix_ptr += _width;
    }

    $DBG printf("copied img\n");
}

Image::Image(Image&& other) {
    
    if (&other == this) {
        throw std::runtime_error("incorrect assignment");
    }

    _width         = other._width;
    _height        = other._height;
    _buf_size      = other._buf_size;
    _info_size     = other._info_size;
    _pixbuf_offset = other._pixbuf_offset;

    buf = other.buf;
    other.buf = nullptr;

    pixbuf = other.pixbuf;
    other.pixbuf = nullptr;

    if (reinterpret_cast<uintptr_t>(pixbuf) % ALIGN != 0) {
        throw std::runtime_error("misaligned ptr");
    }

    $DBG printf("moved img\n");
}

Image::Image(FILE* in_fd) :
        _info_size(0) {

    fseek(in_fd, 0, SEEK_END);
    _buf_size = ftell(in_fd);

    fseek(in_fd, 10, SEEK_SET);
    fread(reinterpret_cast<char*>(&_info_size), sizeof(uint32_t), 1, in_fd);
    fseek(in_fd, 0, SEEK_SET);

    const size_t align_delta = ALIGN - _info_size % ALIGN;

    _pixbuf_offset = _info_size + align_delta;
    _buf_size  += align_delta;

    $DBG printf("info_size     = %lu\n"
                "buf_size      = %lu\n"
                "pixbuf_offset = %lu\n",
                _info_size, _buf_size, _pixbuf_offset);

    buf = new(std::align_val_t{ALIGN}) char[_buf_size]; 

    fread(buf,                  sizeof(char), _info_size,                 in_fd);
    fread(buf + _pixbuf_offset, sizeof(char), _buf_size - _pixbuf_offset, in_fd);

    _width  = *reinterpret_cast<uint32_t*>(&buf[18]);
    _height = *reinterpret_cast<uint32_t*>(&buf[22]);

    pixbuf = new uint32_t*[_height];

    uint32_t* pix_ptr = reinterpret_cast<uint32_t*>(buf + _pixbuf_offset);

    $DBG printf("pixbuf starts at %p\n"
                "align of ptr = %lu\n", 
                pix_ptr, alignof(*pix_ptr));

    assert(reinterpret_cast<uintptr_t>(pix_ptr) % ALIGN == 0);

    for (size_t y = 0; y < _height; ++y) {
        pixbuf[y] = pix_ptr;
        pix_ptr += _width;
    }
}

Image::~Image() {
    ::operator delete(buf, std::align_val_t{ALIGN});
    $DBG printf("img deleted\n");
}

size_t Image::width() const {
    return _width;
}

size_t Image::height() const {
    return _height;
}

void Image::write(FILE* out_fd) const {
    fwrite(buf,                  sizeof(char), _info_size,                 out_fd);
    fwrite(buf + _pixbuf_offset, sizeof(char), _buf_size - _pixbuf_offset, out_fd);
    $DBG printf("img written\n");
}
