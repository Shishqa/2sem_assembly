#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "simple_vector/vector.hpp"
#include "elf_custom.hpp"
#include "utils.hpp"
#include "instructions/config/defines.hpp"

void sas_to_elf64(const char* in_file, const char* out_file) {

    char* buffer = nullptr;
    const size_t file_size = read_from_file(in_file, buffer);

    static const size_t SAS_SIGN_SIZE = 0x19;

    try {

        if (strncmp(buffer, "SHSH", 4)) {
            throw std::runtime_error("unknown input file format");
        }
 
        Vector<Instruction> x64bit_codes = std::move(encode_sas(buffer    + SAS_SIGN_SIZE, 
                                                                file_size - SAS_SIGN_SIZE));
     
        optimize_codes(x64bit_codes);

        write_elf(out_file, x64bit_codes);

        delete[] buffer;

    } catch (const std::runtime_error& ex) {
        delete[] buffer;
        throw ex;
    }
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


Vector<Instruction> encode_sas(const char* buf, const size_t& buf_size) {

    Vector<Instruction> codes;

    Instruction::set_buf_begin(buf);
    Instruction::resize_offsets(buf_size);

    const char* buf_end = buf + buf_size;

    for (const char* op_ptr = buf; op_ptr < buf_end; ++op_ptr) {

        codes.push_back(std::move(Instruction(op_ptr)));

        op_ptr += codes[codes.size() - 1].n_args() * sizeof(Argument);
    }

    return codes;
}


void optimize_codes(Vector<Instruction>& codes) {

    size_t n_codes = codes.size();
    const char* buf_begin = Instruction::get_buf_begin();

    for (size_t i = 0; i < n_codes; ++i) {
           
        if (*codes[i].opcode == _PUSH && *codes[i + 1].opcode == _POP) {
            
            *const_cast<char*>(codes[i].opcode) = _MOV;

            codes[i].arg.resize(2);
            codes[i].arg[1] = codes[i + 1].arg[0];

            codes[++i].mute();
    
            std::cout << "optimized push-pop";

        } else if (*codes[i].opcode == _JMP && 
                   codes[i].arg[0]->val - (codes[i].opcode - buf_begin) ==
                   sizeof(Argument) + 1) {
            codes[i].mute();
        }
            
    }
}


void write_elf(const char* path, const Vector<Instruction>& codes) {

    std::ofstream out_fs(path, std::ofstream::binary);

    if (out_fs.rdstate() & std::ofstream::failbit) {
        throw std::runtime_error("can't write to specified output path");
    }

    char* buffer = nullptr;
    const size_t buf_size = codes_to_buf(codes, buffer);
 
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

size_t codes_to_buf(const Vector<Instruction>& codes, char*& buf) {

    size_t buf_size = 100000; 

    buf = new char[buf_size];
    char* buf_ptr = buf;

    try {

        buf_ptr = Instruction::write_preamble(buf_ptr);

        for (size_t i = 0; i < codes.size(); ++i) {
            buf_ptr = codes[i].write(buf_ptr);
        }

        for (size_t i = 0; i < codes.size(); ++i) {
            codes[i].fix_jmp();
        }

    } catch (const std::runtime_error& ex) {
        delete[] buf;
        throw ex;
    }

    return buf_ptr - buf;
}
