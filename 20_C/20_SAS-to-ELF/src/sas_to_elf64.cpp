#include <iostream>
#include <fstream>
#include <cstring>

#include "SMUtils.hpp"

typedef char    BYTE;
typedef short   WORD;
typedef int     DWORD;
typedef long    QWORD;

#pragma pack(1)
struct elf_header {

    const BYTE  TEXT_SHIELD         = 0x7F;             // editors, it is not a text file!
    const BYTE  ELF_ASCII[3]        = {'E','L','F'};    // format specification

    const BYTE  BITNESS             = 2;        // 64bit 
    const BYTE  NUMBER_FORMAT       = 1;        // little endian

    const BYTE  ELF_HEADER_VER      = 1;        // ????
    const BYTE  OS_ABI              = 0;        // system V

    const QWORD PADDING             = 0;        // unused

    const WORD  ELF_TYPE            = 2;        // executable
    const WORD  INSTRUCTION_SET     = 0x3E;     // x86_64 instruction set
    const DWORD ELF_VERSION         = 1;        // ????

    /*!*/ QWORD ENTRY_POS           = 0;
    /*!*/ QWORD PROGRAM_HEADER_POS  = 0;
    /*!*/ QWORD SECTION_HEADER_POS  = 0;

    const DWORD FLAGS               = 0;        // ????
    
    /*!*/ WORD  HEADER_SIZE         = sizeof(elf_header);
    /*!*/ WORD  ENTRY_PROG_SIZE     = 0;
    /*!*/ WORD  NUM_OF_PROG_ENTRIES = 0;
    /*!*/ WORD  ENTRY_SECT_SIZE     = 0;
    /*!*/ WORD  NUM_OF_SECT_ENTRIES = 0;
    /*!*/ WORD  SECT_TABLE_IDX      = 0;        // ???
};
#pragma pack()

const elf_header ELF_HEADER;

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

    out_fs.write(reinterpret_cast<const char*>(&ELF_HEADER), sizeof(ELF_HEADER));

    out_fs.close();
}




