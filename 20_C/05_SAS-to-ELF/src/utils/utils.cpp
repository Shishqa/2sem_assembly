#include <iostream>
#include <fstream>
#include <cstring>

#include "elf_custom.hpp"
#include "utils.hpp"

void sas_to_elf64(const char* in_file, const char* out_file) {

    char* buffer = nullptr;
    const size_t file_size = read_from_file(in_file, buffer);

    if (strncmp(buffer, "SHSH", 4)) {
        delete[] buffer;
        throw std::runtime_error("unknown input file format");
    }

    const size_t SAS_SIGN_SIZE = 0x19;

    Instruction* x64bit_codes = nullptr;
    const size_t n_codes = sas_to_arr_of_codes(buffer + SAS_SIGN_SIZE, 
                                               file_size - SAS_SIGN_SIZE,
                                               x64bit_codes);
    delete[] buffer;

    write_elf(out_file, x64bit_codes, n_codes);

    std::cout << "deleting codes\n";

    ::operator delete(x64bit_codes);
}


size_t read_from_file(const char* path, char*& buf) {

    std::ifstream in_fs(path, std::ifstream::binary);

    if (in_fs.rdstate() & std::ifstream::failbit) {
        throw std::runtime_error("can't open input file");
    }

    std::filebuf* p_buf = in_fs.rdbuf();

    const size_t file_size = p_buf->pubseekoff(0, in_fs.end, in_fs.in);
    p_buf->pubseekpos(0, in_fs.in);

    buf = new char[file_size];
    p_buf->sgetn(buf, file_size);

    in_fs.close();

    return file_size;
}


size_t arr_of_codes_to_buf(const Instruction* codes, const size_t& n_codes,
                           char*& buf) {

    size_t buf_size = 0; 

    for (size_t i = 0; i < n_codes; ++i) {
        buf_size += codes[i].size();
    }

    buf = new char[buf_size];
    char* buf_ptr = buf;

    for (size_t i = 0; i < n_codes; ++i) {
        buf_ptr += codes[i].write(buf_ptr);
        codes[i].~Instruction();
    }

    return buf_size;
}


void write_elf(const char* path, const Instruction* codes, 
                      const size_t& n_codes) {

    char* buffer = nullptr;
    const size_t buf_size = arr_of_codes_to_buf(codes, n_codes, buffer);

    std::ofstream out_fs(path, std::ofstream::binary);

    if (out_fs.rdstate() & std::ofstream::failbit) {
        throw std::runtime_error("can't write to specified output path");
    }
    
    ProgHeader p_header = {};
    ElfHeader  e_header = {};

    e_header.entry_addr = p_header.P_VADDR + sizeof(ElfHeader) + sizeof(ProgHeader);
    e_header.ph_size    = sizeof(ProgHeader);

    p_header.p_filesz = sizeof(ElfHeader) + sizeof(ProgHeader) + buf_size;
    p_header.p_memsz  = p_header.p_filesz;

    out_fs.write(reinterpret_cast<const char*>(&e_header), sizeof(ElfHeader));
    out_fs.write(reinterpret_cast<const char*>(&p_header), sizeof(ProgHeader));
    out_fs.write(buffer, buf_size);

    delete[] buffer;

    out_fs.close();
}


size_t sas_to_arr_of_codes(const char* buf, const size_t& buf_size,
                                  Instruction*& codes) {

    codes = reinterpret_cast<Instruction*>(::operator new(buf_size * sizeof(Instruction)));

    std::cout << "allocated codes[" << codes << "] of size: " << buf_size << "\n";

    size_t n_codes = 0;

    const char* buf_end = buf + buf_size;
    for (const char* op_ptr = buf; op_ptr < buf_end; ++op_ptr) {
        std::cout << "created instruction at " << codes + n_codes << "\n";
        new(codes + n_codes) Instruction(op_ptr);
        op_ptr += codes[n_codes].n_args() * sizeof(Argument);
        ++n_codes;
    }

    return n_codes;
}
