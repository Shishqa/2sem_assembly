#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {

    CRACK_SUCCESS          = 0,
    CRACK_WRONG_SIZE       = 1,
    CRACK_WRONG_CHECKSUM   = 2,
    CRACK_INPUT_PROBLEM    = 3,
    CRACK_OUTPUT_PROBLEM   = 4

};

const char DEFAULT_FILE_OUTPUT[7] = "hacked";

void print_help();

int crack(char* input_file, char* output_file);


int main(int argc, char** argv) {

    if (argc < 2) {

        print_help();

        return 0;
    }

    char* file_input  = argv[1];
    char* file_output = DEFAULT_FILE_OUTPUT;

    if (argc > 2) {
        
        if (strlen(argv[2]) != 2 || strncmp(argv[2], "-o", 2)) {

            printf("# Unknown flag: %s\n", argv[2]);
            print_help();

            return 0;
        }

        if (argc == 3) {
            
            printf("# No argument for -o\n");
            print_help();

            return 0;
        }

        file_output = argv[3];

    }

    int res_code = crack(file_input, file_output);

    switch (res_code) {

        case CRACK_SUCCESS:
            printf("# Program cracked: %s\n", file_output);
            break;
        case CRACK_WRONG_SIZE:
            printf("# Size of input file is incorrect!\n");
            break;
        case CRACK_WRONG_CHECKSUM:
            printf("# Checksum of input file is incorrect!\n");
            break;
        case CRACK_INPUT_PROBLEM:
            printf("# Can't open %s\n", file_input);
            break;
        case CRACK_OUTPUT_PROBLEM:
            printf("# Can't write to %s\n", file_output);
            break;
        default:
            printf("###BAD_CODE###\n");
            break;

    }

    return 0;
}


void print_help() {

    printf("# Usage: crk <file> [-o <filename>]\n"
           "# default output name: %s\n", DEFAULT_FILE_OUTPUT);
}


int crack(char* file_input, char* file_output) {

    const long NOMINAL_CHECKSUM = 31975;
    const size_t NOMINAL_SIZE = 9480;

    FILE* fd = fopen(file_input, "r");

    if (!fd) {

        return CRACK_INPUT_PROBLEM;
    }

    fseek(fd, 0, SEEK_END);
    size_t f_size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if (f_size != NOMINAL_SIZE) {

        fclose(fd);
        return CRACK_WRONG_SIZE;
    }

    char* buffer = (char *) calloc(f_size, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), f_size, fd);

    fclose(fd);

    unsigned long checksum = 0;

    for (size_t i = 0; i < f_size; ++i) {

        checksum += buffer[i];
    }

    if (checksum != NOMINAL_CHECKSUM) {

        free(buffer);
        return CRACK_WRONG_CHECKSUM;
    }

    for (size_t i = 4096; i <= 4105; ++i) {

        buffer[i] = (char)0x90;
    }
    for (size_t i = 4109; i <= 4113; ++i) {

        buffer[i] = (char)0x90;
    }

    buffer[4206] = (char)0x74;
    buffer[4207] = (char)0x43;

    fd = fopen(file_output, "w");

    if (!fd) {
        
        free(buffer);
        return CRACK_OUTPUT_PROBLEM;
    }

    fwrite(buffer, sizeof(char), f_size, fd);

    fclose(fd);

    free(buffer);

    return CRACK_SUCCESS;
}
