//
// Created by shishqa on 4/30/20.
//

#ifndef INC_05_IMG_BLEND_IMAGE_H
#define INC_05_IMG_BLEND_IMAGE_H

#include <iostream>

class Image {
public:

    Image() = delete;

    explicit Image(const char* path);

    Image(const Image& other);

    ~Image();

    [[nodiscard]] size_t width() const;
    [[nodiscard]] size_t height() const;

    uint32_t** pixbuf;

    friend std::ostream& operator<<(std::ostream& out, const Image& img);

private:

    static const size_t MAX_PREAMBLE_SIZE = 200;
    static const size_t MAX_HEIGHT        = 1000;
    static const size_t MAX_WIDTH         = 1000;

    char* info;

    size_t _width;
    size_t _height;
    size_t _info_size;
};

#endif //INC_05_IMG_BLEND_IMAGE_H
