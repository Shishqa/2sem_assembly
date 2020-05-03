//
// Created by shishqa on 4/30/20.
//

#include "Image.h"
#include <cstdio>

Image::Image(const Image& other) :
        _width(other._width),
        _height(other._height),
        _info_size(other._info_size) {

    info = new char[_info_size];

    for (size_t i = 0; i < _info_size; ++i) {
        info[i] = other.info[i];
    }

    pixbuf = new uint32_t*[_height];

    for (size_t y = 0; y < _height; ++y) {
        pixbuf[y] = new uint32_t[_width];
        for (size_t x = 0; x < _width; ++x) {
            pixbuf[y][x] = other.pixbuf[y][x];
        }
    }
}

Image::Image(const char* path) :
        _info_size(0) {

    FILE* in_fd = fopen(path, "r");

    if (!in_fd) {
        throw std::runtime_error("can't open file");
    }

    fseek(in_fd, 10, SEEK_SET);
    fread(reinterpret_cast<void*>(&_info_size), sizeof(uint32_t), 1, in_fd);
    fseek(in_fd, 0, SEEK_SET);

    info = new char[_info_size];

    fread(reinterpret_cast<void*>(info), sizeof(char), _info_size, in_fd);

    _width = *reinterpret_cast<uint32_t*>(&info[18]);
    _height = *reinterpret_cast<uint32_t*>(&info[22]);

    if (_width > MAX_WIDTH || _height > MAX_HEIGHT) {
        delete info;
        throw std::runtime_error("very big image");
    }

    pixbuf = new uint32_t*[_height];

    for (size_t y = 0; y < _height; ++y) {
        pixbuf[y] = new uint32_t[_width];
        fread(reinterpret_cast<void*>(pixbuf[y]), sizeof(uint32_t), _width, in_fd);
    }

    fclose(in_fd);
}

Image::~Image() {

    delete[] info;

    for (size_t y = 0; y < _height; ++y) {
        delete[] pixbuf[y];
    }

    delete[] pixbuf;
}

size_t Image::width() const {
    return _width;
}

size_t Image::height() const {
    return _height;
}

std::ostream& operator<<(std::ostream& out, const Image& img) {

    for (size_t i = 0; i < img._info_size; ++i) {
        out << img.info[i];
    }

    uint32_t curr_pixel = 0;

    for (size_t y = 0; y < img._height; ++y) {
        for (size_t x = 0; x < img._width; ++x) {

            curr_pixel = img.pixbuf[y][x];

            for (size_t i = 0; i < 4; ++i, curr_pixel >>= 8) {
                out << static_cast<char>(curr_pixel & 0xFF);
            }
        }
    }

    return out;
}