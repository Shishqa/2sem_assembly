#include "Blender.h"

#include <fstream>
#include <cstring>

const char* DEFAULT_OUT_FILE = "blended.bmp";

void print_help();
void print_error(const std::runtime_error& ex);

int main(int argc, char** argv) {

    const char* front_filename  = nullptr;
    const char* back_filename   = nullptr;
    const char* output_filename = DEFAULT_OUT_FILE;

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

            } else {
                throw std::runtime_error("incorrect flag");
            }
        }

        Image front(front_filename);
        Image back(back_filename);
        Image res(back);

        Blender::blend_images_sse(front, back, res);

        std::ofstream fout(output_filename);
        fout << res;
        fout.close();

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

void print_help() {
    std::cout << "  Alpha-Blender help:\n"
                 "\n"
                 "  Usage: ./blend [front] [back] <options>\n"
                 "\n"
                 "  OPTIONS\n"
                 "      -o <name-of-output-file> \n"
                 "          If you want custom output filename\n"
                 "          instead of default ("
                            << DEFAULT_OUT_FILE << ")\n"
                 "\n"
                 "      -sse\n"
                 "          If you want images to be blended with\n"
                 "          intel sse instructions (faster!)\n";
}

void print_error(const std::runtime_error& ex) {
    std::cout << "Error: " << ex.what() << std::endl;
    print_help();
}