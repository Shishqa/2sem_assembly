//
// Created by shishqa on 4/30/20.
//

#include "Blender.h"
#include "Image.h"
#include <nmmintrin.h>

#include <cstdio>

void Blender::blend_images_sse(const Image& front, const Image& back, Image& dest, const size_t& n_runs) {

    const size_t WIDTH = front.width();
    const size_t HEIGHT = front.height();

    if (WIDTH != back.width() || HEIGHT != back.height()) {
        throw std::runtime_error("blending images of different sizes");
    }

    const char X = static_cast<char>(0x80);
    const char F = static_cast<char>(0xFF);

    const __m128i C255 = _mm_set_epi8(0, F, 0, F, 0, F, 0, F, 0, F, 0, F, 0, F, 0, F);

    for (size_t k = 0; k < n_runs; ++k) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            for (size_t x = 0; x < WIDTH; x += 4) {

                __m128i front_l = _mm_load_si128(reinterpret_cast<const __m128i*>(&front.pixbuf[y][x]));
                __m128i back_l = _mm_load_si128(reinterpret_cast<const __m128i*>(&back.pixbuf[y][x]));

                //==========================================================================================================

                /*                 _______________________________________________________________
                 *                  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *      ***_l:    [b0  g0  r0  a0  b1  g1  r1  a1  b2  g2  r2  a2  b3  g3  r3  a3]
                 *                   _____________________________/___/___/___/___/___/___/___/
                 *                  /   /   /   /   /   /   /   /
                 *                 \/  \/  \/  \/  \/  \/  \/  \/
                 *      ***_h:    [b2  g2  r2  a2  b3  g3  r3  a3  xx  xx  xx  xx  xx  xx  xx  xx]
                 *                  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *                 --------------------------------------------------------------
                 */

                __m128i shuffle_high_to_low = _mm_set_epi8(X, X, X, X, X, X, X, X, 15, 14, 13, 12, 11, 10, 9, 8);

                __m128i front_h = _mm_shuffle_epi8(front_l, shuffle_high_to_low);
                __m128i back_h = _mm_shuffle_epi8(back_l, shuffle_high_to_low);

                //==========================================================================================================

                /*                   _______________________________________________________________
                 *                    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *      ***_[l/h]:  [b0  g0  r0  a0  b1  g1  r1  a1  xx  xx  xx  xx  xx  xx  xx  xx]
                 *                   |    \   \   \_  \_  \_  \__ \__________
                 *                   |     \   \    \   \   \___ \________   \_____________
                 *                   |      \   \__  \__ \_____ \_______  \_________       \
                 *                   |       \     \__  \_____ \______  \_____      \       \
                 *                   |        \       \       \       \       \      \       \
                 *      ***_[l/h]:  [b0  xx  g0  xx  r0  xx  a0  xx  b1  xx  g1  xx  r1  xx  a1  xx]
                 *                    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *                   --------------------------------------------------------------
                 */

                front_l = _mm_cvtepi8_epi16(front_l);
                front_h = _mm_cvtepi8_epi16(front_h);

                back_l = _mm_cvtepi8_epi16(back_l);
                back_h = _mm_cvtepi8_epi16(back_h);

                //==========================================================================================================

                /*                  ______________________________________________________________
                 *                   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *      front:     [b0  xx  g0  xx  r0  xx  a0  xx  b1  xx  g1  xx  r1  xx  a1  xx]
                 *                    ______________________|         ______________________|
                 *                   /       |      \       \        /       |      \       \
                 *      alpha:     [a0  xx  a0  xx  a0  xx  a0  xx  a1  xx  a1  xx  a1  xx  a1  xx]
                 *                   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
                 *                 ---------------------------------------------------------------
                 */


                __m128i shuffle_alpha_mask = _mm_set_epi8(X, 14, X, 14, X, 14, X, 14,
                                                          X, 6, X, 6, X, 6, X, 6);

                __m128i alpha_l = _mm_shuffle_epi8(front_l, shuffle_alpha_mask);
                __m128i alpha_h = _mm_shuffle_epi8(front_h, shuffle_alpha_mask);

                //==========================================================================================================

                front_l = _mm_mullo_epi16(front_l, alpha_l);
                front_h = _mm_mullo_epi16(front_h, alpha_h);                     // front *= alpha

                back_l = _mm_mullo_epi16(back_l, _mm_sub_epi16(C255, alpha_l));
                back_h = _mm_mullo_epi16(back_h, _mm_sub_epi16(C255, alpha_h));  // back *= (255 - alpha)

                __m128i sum_l = _mm_add_epi16(front_l, back_l);
                __m128i sum_h = _mm_add_epi16(front_h, back_h);                  // sum = front + back

                //==========================================================================================================

                /*                  _______________________________________________________________
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *      sum_l:     [b0L b0H g0L g0H r0L r0H a0L a0H b1L b1H g1L g1H r1L r1H a1L a1H]
                 *                      /_______/_______/_______/_______/_______/_______/_______/
                 *                    /   /   /   /   /   /   /   /
                 *      sum_l:     [b0H g0H r0H a0H b1H g1H r1H a1H  xx  xx  xx  xx  xx  xx  xx  xx]
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *                 ---------------------------------------------------------------
                 */

                __m128i shuffle_sum_l_mask = _mm_set_epi8(X, X, X, X, X, X, X, X, 15, 13, 11, 9, 7, 5, 3, 1);

                sum_l = _mm_shuffle_epi8(sum_l, shuffle_sum_l_mask);

                /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

                /*                  _______________________________________________________________
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *      sum_h:     [b2L b2H g2L g2H r2L r2H a2L a2H b3L b3H g3L g3H r3L r3H a3L a3H]
                 *                       \_______\_______\_______\_______\_______\_______\_______/
                 *                                                   \   \   \   \   \   \   \   \
                 *      sum_h:     [xx  xx  xx  xx  xx  xx  xx  xx  b2H g2H r2H a2H b3H g3H r3H a3H]
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *                 ---------------------------------------------------------------
                 */

                __m128i shuffle_sum_h_mask = _mm_set_epi8(15, 13, 11, 9, 7, 5, 3, 1, X, X, X, X, X, X, X, X);

                sum_h = _mm_shuffle_epi8(sum_h, shuffle_sum_h_mask);

                //==========================================================================================================

                /*                  _______________________________________________________________
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *      sum_l:     [b0H g0H r0H a0H b1H g1H r1H a1H  xx  xx  xx  xx  xx  xx  xx  xx]
                 *      sum_h:     [xx  xx  xx  xx  xx  xx  xx  xx  b2H g2H r2H a2H b3H g3H r3H a3H]
                 *                   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
                 *                   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
                 *        res:     [b0H g0H r0H a0H b1H g1H r1H a1H b2H g2H r2H a2H b3H g3H r3H a3H]
                 *                   0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                 *                 ---------------------------------------------------------------
                 */

                __m128i res = _mm_or_si128(sum_h, sum_l);

                //==========================================================================================================

                _mm_storeu_si128(reinterpret_cast<__m128i*>(&dest.pixbuf[y][x]), res);
            }
        }
    }
}

void Blender::blend_images(const Image& front, const Image& back, Image& dest, const size_t& n_runs) {

    const size_t WIDTH = front.width();
    const size_t HEIGHT = front.height();

    if (WIDTH != back.width() || HEIGHT != back.height()) {
        throw std::runtime_error("blending images of different sizes");
    }

    unsigned int alpha_mask = 0xFF000000;
    unsigned short alpha = 0x00;

    unsigned short tmp = 0x0000;

    for (size_t k = 0; k < n_runs; ++k) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            for (size_t x = 0; x < WIDTH; ++x) {

                alpha = (front.pixbuf[y][x] & alpha_mask) >> 24;

                for (unsigned int mask = 0x000000FF, i = 0; i < 4; mask <<= 8, ++i) {

                    tmp = ((front.pixbuf[y][x] & mask) >> 8 * i) * static_cast<unsigned short>(alpha);
                    tmp += ((back.pixbuf[y][x] & mask) >> 8 * i) * static_cast<unsigned short>(255 - alpha);
                    tmp >>= 8;
        
                    dest.pixbuf[y][x] &= (UINT32_MAX - mask); 
                    dest.pixbuf[y][x] |= (tmp << 8 * i);
                }
            }
        }
    }
}
