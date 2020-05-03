#include <iostream>
#include <cstring>

#include "SMUtils.hpp";

const char* DEFAULT_OUT_FILE = "a.out";

void print_help();
void print_error(const std::runtime_error& ex);

int main(int argc, char** argv) {

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

            } else if (!strncmp(argv[arg_it], "-sse", 5)) {
                
                std::cout << "using sse\n";

            } else if (!strncmp(argv[arg_it], "-q", 3)) {

                std::cout << "quiet\n";

            } else if (!strncmp(argv[arg_it], "-f", 3)) {

                std::cout << "force translation\n";

            } else {

                throw std::runtime_error("unknown flag specified");

            }
        }

        SMU::sas_to_elf64(input_filename, output_filename);

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

void print_error(const std::runtime_error& ex) {

    std::cout << "   " << ex.what() << "\n\n";
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
                 "      -sse                                                \n"
                 "          to prefer sse instructions when it's possible   \n"
                 "                                                          \n"
                 "      -q                                                  \n"
                 "          to mute warnings                                \n"
                 "                                                          \n"
                 "      -f {UNSAFE}                                         \n"
                 "          force translation: leave nop command where      \n"
                 "          there is no suitable x86_64 instruction         \n"
                 "          instead of terminating the process              \n"
                 "                                                          \n";
}
