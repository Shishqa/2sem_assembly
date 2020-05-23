#include "Blender.h"

#include <fstream>
#include <cstring>
#include <cctype>

const char* DEFAULT_OUT_FILENAME = "blended.bmp";

FILE* safe_fopen(const char* filename, const char* mode);
Image read_image(const char* filename);
void write_image(const char* filename, const Image& img);

void print_help();
void print_error(const std::runtime_error& ex);

int main(int argc, const char** argv) {

    const char* front_filename  = nullptr;
    const char* back_filename   = nullptr;
    const char* out_filename = DEFAULT_OUT_FILENAME;

    bool no_sse = false;
    size_t n_runs = 1;

    try {

        if (argc == 2 && !strncmp(argv[1], "-h", 2)) {

            print_help();
            return 0;
            
        } else if (argc >= 3) {

            front_filename = argv[1];
            back_filename  = argv[2];

        } else {
            throw std::runtime_error("incorrect number of args");
        }

        for (int arg_it = 3; arg_it < argc; ++arg_it) {

            if (!strncmp(argv[arg_it], "-o", 2)) {

                if (arg_it < argc - 1) {

                    out_filename = argv[++arg_it];

                } else {
                    throw std::runtime_error("no -o flag argument specified");
                }

            } else if (!strncmp(argv[arg_it], "-c", 2)) {

                if (arg_it < argc - 1) {

                    ++arg_it;

                    if (*argv[arg_it] < '0' || *argv[arg_it] > '9') {
                        throw std::runtime_error("-c flag argument must be number");
                    }

                    sscanf(argv[arg_it], "%lu", &n_runs);

                } else {
                    throw std::runtime_error("no -c flag argument specified");
                }

            } else if (!strncmp(argv[arg_it], "--no-sse", 8)) {

                no_sse = true;

            } else {
                throw std::runtime_error("incorrect flag");
            }
        }

        Image front = std::move(read_image(front_filename));
        Image back  = std::move(read_image(back_filename));
        Image res   = back;

        if (no_sse) {
            Blender::blend_images(front, back, res, n_runs);
        } else { 
            Blender::blend_images_sse(front, back, res, n_runs);
        }

        write_image(out_filename, res);

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

FILE* safe_fopen(const char* filename, const char* mode) {

    FILE* fd = fopen(filename, mode);

    if(!fd) {
        throw std::runtime_error("can't open file");
    }

    return fd;
}

Image read_image(const char* filename) {
    
    FILE* in_fd = safe_fopen(filename, "rb");

    Image img(in_fd);

    fclose(in_fd);

    return img;
}

void write_image(const char* filename, const Image& img) {
    
    FILE* out_fd = safe_fopen(filename, "wb");

    img.write(out_fd);
    
    fclose(out_fd);
}

void print_help() {
    printf( "  Alpha-Blender help:                                     \n"
            "                                                          \n"
            "  Usage: ./blend [front] [back] <options>                 \n"
            "                                                          \n"
            "  OPTIONS                                                 \n"
            "      -h                                                  \n"
            "          Print useful help                               \n"
            "                                                          \n"
            "      -o [name-of-output-file]                            \n"
            "          If you want custom output filename              \n"
            "          instead of default (%s)                         \n"
            "                                                          \n"
            "      --no-sse                                            \n"
            "          If you want images to be blended without        \n"
            "          using intel SSE instructions (slower)           \n"
            "                                                          \n"
            "      -c [number]                                         \n"
            "          If you want profile performance, render can be  \n"
            "          runned for <number> times (default is 1)        \n"
            "                                                          \n",
            DEFAULT_OUT_FILENAME);
}

void print_error(const std::runtime_error& ex) {
    fprintf(stderr, 
            "\033[1;31mError: \033[0m%s\n"
            "Use \033[1;33m./blend -h\033[0m for more information\n", 
             ex.what());
}
