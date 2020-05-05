#include "Blender.h"

#include <fstream>
#include <cstring>
#include <cctype>

const char* DEFAULT_OUT_FILE = "blended.bmp";

void print_help();
void print_error(const std::runtime_error& ex);

int main(int argc, char** argv) {

    const char* front_filename  = nullptr;
    const char* back_filename   = nullptr;
    const char* output_filename = DEFAULT_OUT_FILE;

    bool no_sse = false;
    size_t n_runs = 1;

    try {
        if (argc >= 3) {
            front_filename = argv[1];
            back_filename  = argv[2];
        } else {
            throw std::runtime_error("incorrect number of args");
        }

        for (int arg_it = 3; arg_it < argc; ++arg_it) {

            if (!strncmp(argv[arg_it], "-o", 2)) {

                if (arg_it < argc - 1) {

                    output_filename = argv[++arg_it];

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

            } else if (!strncmp(argv[arg_it], "-no-sse", 7)) {

                no_sse = true;

            } else {
                throw std::runtime_error("incorrect flag");
            }
        }

        Image front(front_filename);
        Image back(back_filename);
        Image res(back);

        if (no_sse) {
            Blender::blend_images_no_sse(front, back, res, n_runs);
        } else {
            Blender::blend_images_sse(front, back, res, n_runs);
        }

        std::ofstream fout(output_filename);
        fout << res;
        fout.close();

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

void print_help() {
    std::cout << "  Alpha-Blender help:                                     \n"
                 "                                                          \n"
                 "  Usage: ./blend [front] [back] <options>                 \n"
                 "                                                          \n"
                 "  OPTIONS                                                 \n"
                 "      -o <name-of-output-file>                            \n"
                 "          If you want custom output filename              \n"
                 "          instead of default (" << DEFAULT_OUT_FILE << ") \n"
                 "                                                          \n"
                 "      -no-sse                                             \n"
                 "          If you want images to be blended without        \n"
                 "          using intel SIMD instructions (slower)          \n"
                 "                                                          \n"
                 "      -c <number>                                         \n"
                 "          If you want profile performance, render can be  \n"
                 "          runned for <number> times (default is 1)        \n";
}

void print_error(const std::runtime_error& ex) {
    std::cout << "  " << ex.what() << std::endl;
    print_help();
}
