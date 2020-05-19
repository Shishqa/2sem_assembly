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

        if (!strncmp(argv[1], "--help", 6)) {
            print_help();
            return 0;
        }

        input_filename = argv[1];

        for (int arg_it = 2; arg_it < argc; ++arg_it) {

            if (!strncmp(argv[arg_it], "-o", 3)) {

                if (arg_it < argc - 1) {
                    output_filename = argv[++arg_it];
                } else {
                    throw std::runtime_error("no -o flag path specified");
                }

            } else if (!strncmp(argv[arg_it], "--optimize", 10)) {
                
                Utils::use_optimization(true); 

            } else if (!strncmp(argv[arg_it], "--quiet", 7)) {

                Utils::extra_info(false);

            } else {

                throw std::runtime_error("unknown flag specified");

            }
        }

        Utils::sas_to_elf64(input_filename, output_filename);

    } catch (const std::runtime_error& ex) {
        print_error(ex);
    }

    return 0;
}

void print_error(const std::runtime_error& ex) {

    fprintf(stderr, "\033[1;31mError: \033[0m%s\n"
                    "Use \033[1;33m./sastoelf --help\033[0m for more information\n", 
                    ex.what());
}

void print_help() {

    printf( "   SAS to elf x86_64 binary converter HELP                \n"
            "                                                          \n"
            "   Usage: ./sastoelf [input.bin] <options>                \n"
            "                                                          \n"
            "      --help                                              \n"
            "          to show this useful information                 \n"
            "                                                          \n"
            "      -o [output path]                                    \n"
            "          if you want to specify custom output path       \n"
            "                                                          \n"
            "      --optimize                                          \n"
            "          optimize program                                \n"
            "                                                          \n"
            "      --quiet                                             \n"
            "          to mute warnings                                \n" );
}
