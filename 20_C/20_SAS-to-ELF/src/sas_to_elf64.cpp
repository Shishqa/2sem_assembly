#include <iostream>
#include <fstream>
#include <cstring>

#include "SMUtils.hpp"

typedef char    BYTE;
typedef short   WORD;
typedef int     DWORD;
typedef long    QWORD;

#pragma pack(1)
struct program_header {

    /*!*/ DWORD SEGMENT_TYPE        = 1;
    /*!*/ DWORD FLAGS               = 5;
    /*!*/ QWORD P_OFFSET            = 0;
    /*!*/ QWORD P_VADDR             = 0x400000;
    /*!*/ QWORD UNDEF               = 0;
    /*!*/ QWORD P_FILESZ            = 0;
    /*!*/ QWORD P_MEMSZ             = 0;
    /*!*/ QWORD ALIGNMENT           = 0x1000;
};

struct elf_header {

    const BYTE  TEXT_SHIELD         = 0x7F;             // editors, it is not a text file!
    const BYTE  ELF_ASCII[3]        = {'E','L','F'};    // format specification

    const BYTE  CLASS               = 2;                // ELF64 
    const BYTE  NUMBER_FORMAT       = 1;                // little endian

    const BYTE  ELF_HEADER_VER      = 1;                // 
    const BYTE  OS_ABI              = 0;                // System V

    const QWORD PADDING             = 0;                // unused

    const WORD  ELF_TYPE            = 2;                // EXEC
    const WORD  INSTRUCTION_SET     = 0x3E;             // x86_64 instruction set
    const DWORD ELF_VERSION         = 1;                // 

    /*!*/ QWORD entry_addr          = 0x400000 + sizeof(elf_header) + sizeof(program_header);
    /*!*/ QWORD ph_offset           = sizeof(elf_header);
    /*!*/ QWORD sh_offset           = 0;

    const DWORD FLAGS               = 0;                // ????
    
    /*!*/ WORD  header_size         = sizeof(elf_header);
    /*!*/ WORD  ph_size             = sizeof(program_header);
    /*!*/ WORD  num_of_ph           = 1;
    /*!*/ WORD  sh_size             = 0;
    /*!*/ WORD  num_of_sh           = 0;
    /*!*/ WORD  sh_str_table_idx    = 0;                
};

struct return_0 {

    const BYTE    mov_rax           = 0xB8;
    const DWORD   x3C               = 0x3C;
    const BYTE    xor_rdi_rdi[3]    = {0x48, 0x31, static_cast<char>(0xFF)};
    const BYTE    syscall[2]        = {0x0F, 0x05};

};
#pragma pack()

const elf_header        ELF_HEADER;
      program_header    PROG_HEADER;
const return_0          RET;

void SMU::sas_to_elf64(const char* in_file, const char* out_file) {

    std::ifstream in_fs(in_file, std::ifstream::binary);

    if (in_fs.rdstate() & std::ifstream::failbit) {
        throw std::runtime_error("can't open input file");
    }

    std::filebuf* p_buf = in_fs.rdbuf();

    const size_t file_size = p_buf->pubseekoff(0, in_fs.end, in_fs.in);
    p_buf->pubseekpos(0, in_fs.in);

    char* buffer = new char[file_size];
    p_buf->sgetn(buffer, file_size);

    in_fs.close();

    if (strncmp(buffer, "SHSH", 4)) {
        throw std::runtime_error("unknown input file format");
    }

    delete[] buffer;

    std::ofstream out_fs(out_file, std::ofstream::binary);

    if (out_fs.rdstate() & std::ofstream::failbit) {
        throw std::runtime_error("can't write to specified output path");
    }

    PROG_HEADER.P_FILESZ = sizeof(elf_header) + sizeof(program_header) + sizeof(return_0);
    PROG_HEADER.P_MEMSZ = PROG_HEADER.P_FILESZ;

    out_fs.write(reinterpret_cast<const char*>(&ELF_HEADER), sizeof(ELF_HEADER));
    out_fs.write(reinterpret_cast<const char*>(&PROG_HEADER), sizeof(PROG_HEADER));
    out_fs.write(reinterpret_cast<const char*>(&RET), sizeof(RET));

    out_fs.close();
}




