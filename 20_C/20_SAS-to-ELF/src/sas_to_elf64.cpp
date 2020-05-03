#include <iostream>
#include <fstream>
#include <cstring>

#include "Elf.hpp"
#include "SMUtils.hpp"

const size_t MAX_BUF_SIZE = 10000;

size_t alloc_n_read_from_file(const char* path, char*& buf);
void   write_elf_to_file(const char* path, const char* buf, const size_t& buf_size);

void SMU::sas_to_elf64(const char* in_file, const char* out_file) {
    
    char* sas_buffer = nullptr;
    const size_t file_size = alloc_n_read_from_file(in_file, sas_buffer);

    if (strncmp(sas_buffer, "SHSH", 4)) {
        delete[] sas_buffer;
        throw std::runtime_error("unknown input file format");
    }

    char* x64_buffer = new char[MAX_BUF_SIZE];
    size_t x64_size  = SMU::sas_to_x86_64(x64_buffer, sas_buffer);

    delete[] sas_buffer;

    write_elf_to_file(out_file, x64_buffer, x64_size);

    delete[] x64_buffer;
}


size_t alloc_n_read_from_file(const char* path, char*& buf) {

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


void  write_elf_to_file(const char* path, const char* buf, const size_t& buf_size) {

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
    out_fs.write(buf, buf_size);

    out_fs.close();
}
