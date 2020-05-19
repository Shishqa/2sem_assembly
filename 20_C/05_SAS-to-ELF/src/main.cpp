#include <iostream>
#include <cstring>

#include "utils/utils.hpp"

const char* DEFAULT_OUT_FILE = "a.out";

void print_help();
void print_error(const std::runtime_error& ex);

int main(int argc, const char** argv) {

    const char* input_filename  = nullptr;
    const char* output_filename = DEFAULT_OUT_FILE; 

    try {

        if (argc < 2) {
            throw std::runtime_error("no input specified");
        }

        input_filename = argv[1];

        std::cout << "input: " << input_filename << "\n";

        for (int arg_it = 2; arg_it < argc; ++arg_it) {

            if (!strncmp(argv[arg_it], "-o", 3)) {

                if (arg_it < argc - 1) {
                    output_filename = argv[++arg_it];
                    std::cout << "writing to " << output_filename << "\n";
                } else {
                    throw std::runtime_error("no -o flag path specified");
                }

            } else if (!strncmp(argv[arg_it], "--optimize", 10)) {
                
                std::cout << "using sse\n";

            } else if (!strncmp(argv[arg_it], "--quiet", 7)) {

                std::cout << "quiet\n";

            } else if (!strncmp(argv[arg_it], "--force", 7)) {

                std::cout << "force translation\n";

            } else {

                throw std::runtime_error("unknown flag specified");

            }
        }

        sas_to_elf64(input_filename, output_filename);

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

void print_error(const std::runtime_error& ex) {

    std::cerr << "   " << ex.what() << "\n\n";
    print_help();
}

void print_help() {

    std::cout << "   SAS to elf x86_64 binary converter HELP                \n"
                 "                                                          \n"
                 "   Usage: ./sastoelf64 [input.bin] <options>              \n"
                 "                                                          \n"
                 "      -o [output path]                                    \n"
                 "          if you want to specify custom output path       \n"
                 "                                                          \n"
                 "      --optimize                                          \n"
                 "          optimize program                                \n"
                 "                                                          \n"
                 "      --quiet                                             \n"
                 "          to mute warnings                                \n"
                 "                                                          \n"
                 "      --force {UNSAFE}                                    \n"
                 "          force translation: leave nop command where      \n"
                 "          there is no suitable x86_64 instruction         \n"
                 "          instead of terminating the process              \n"
                 "                                                          \n";
}
