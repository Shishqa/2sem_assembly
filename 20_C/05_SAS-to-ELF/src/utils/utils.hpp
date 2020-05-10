#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdio>

#include "instructions/instruction.hpp" 
#include "instructions/argument.hpp"

/* sas_to_elf64:
 *      Read sas binary file from <in_path>,
 *      convert it to x86-64 elf format and
 *      write to <out_path>
 */
void sas_to_elf64(const char* in_path, const char* out_path);

/* read_from_file:
 *      Read binary information from file at <path>,
 *      allocate buffer for info and store it there
 */ 
size_t read_from_file(const char* path, char*& buf);

//
void write_elf(const char* path, const Instruction* codes, const size_t& n_codes);
    
size_t arr_of_codes_to_buf(const Instruction* codes, const size_t& n_codes, 
                           char*& buf);

//
size_t sas_to_arr_of_codes(const char* buf, const size_t& buf_size, 
                           Instruction*& codes);

#endif //UTILS_HPP
