//
// Created by shishqa on 4/30/20.
//

#ifndef INC_05_IMG_BLEND_BLENDER_H
#define INC_05_IMG_BLEND_BLENDER_H

#include "Image.h"

class Blender {

public:
    static void blend_images_sse(const Image& front, const Image& back, Image& dest, const size_t& n_runs = 1);

    static void blend_images_no_sse(const Image& front, const Image& back, Image& dest, const size_t& n_runs = 1);
};

#endif //INC_05_IMG_BLEND_BLENDER_H
