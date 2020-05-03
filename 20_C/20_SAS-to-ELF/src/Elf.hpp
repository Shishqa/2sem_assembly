#ifndef ELF_HPP
#define ELF_HPP

typedef char    BYTE;
typedef short   WORD;
typedef int     DWORD;
typedef long    QWORD;

const QWORD LOAD_ADDR = 0x400000;

#pragma pack(1)
struct ElfHeader {

    const BYTE  TEXT_SHIELD         = 0x7F;                 // editors, it is not a text file!
    const BYTE  ELF_ASCII[3]        = {'E','L','F'};        // format specification

    const BYTE  CLASS               = 2;                    // ELF64 
    const BYTE  NUMBER_FORMAT       = 1;                    // little endian

    const BYTE  ELF_HEADER_VER      = 1;                    // 
    const BYTE  OS_ABI              = 0;                    // System V

    const QWORD PADDING             = 0;                    // unused

    const WORD  ELF_TYPE            = 2;                    // EXEC
    const WORD  INSTRUCTION_SET     = 0x3E;                 // x86_64 instruction set
    const DWORD ELF_VERSION         = 1;                    // 

    /*!*/ QWORD entry_addr          = 0;                    // initial rip
    const QWORD ph_offset           = sizeof(ElfHeader);    //  
    const QWORD SH_OFFSET           = 0;                    // no segment headers

    const DWORD FLAGS               = 0;                    // no flags 
    
    const WORD  HEADER_SIZE         = sizeof(ElfHeader);   
    /*!*/ WORD  ph_size             = 0;
    const WORD  PH_NUM              = 1;
    const WORD  SH_SIZE             = 0;                    // no segment headers
    const WORD  SH_NUM              = 0;
    const WORD  SH_TABLE_IDX        = 0;                
};

struct ProgHeader {
    const DWORD SEGMENT_TYPE        = 1;                    // LOAD
    const DWORD FLAGS               = 5;                    // R X
    const QWORD P_OFFSET            = 0;                    //
    const QWORD P_VADDR             = LOAD_ADDR;            // load to LOAD_ADDR
    const QWORD UNDEF               = 0;                    // real memory addr is unused
    /*!*/ QWORD p_filesz            = 0;
    /*!*/ QWORD p_memsz             = 0;
    const QWORD ALIGNMENT           = 0x1000;
};
#pragma pack()

#endif //ELF_HPP
