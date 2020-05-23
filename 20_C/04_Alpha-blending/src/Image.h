//
// Created by shishqa on 4/30/20.
//

#ifndef INC_05_IMG_BLEND_IMAGE_H
#define INC_05_IMG_BLEND_IMAGE_H

#include <iostream>

class Image {
public:

    Image() = delete;

    explicit Image(FILE* in_fd);

    Image(const Image& other);

    Image& operator=(const Image& other) = delete;

    Image(Image&& other);

    Image& operator=(Image&& other) = delete;

    ~Image();

    [[nodiscard]] size_t width() const;
    [[nodiscard]] size_t height() const;

    uint32_t** pixbuf;

    void write(FILE* out_fd) const;

private:

    char* buf;

    size_t _width;
    size_t _height;
    size_t _buf_size;
    size_t _info_size;
    size_t _pixbuf_offset;

    static const size_t ALIGN = 16;
};

#endif //INC_05_IMG_BLEND_IMAGE_H
