#ifndef X86_DEFINES_HPP
#define X86_DEFINES_HPP

enum REGISTERS {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7
};

enum EXT_REGISTERS {
    R8  = 0,
    R9  = 1,
    R10 = 2,
    R11 = 3,
    R12 = 4,
    R13 = 5,
    R14 = 6,
    R15 = 7
};

enum SYSTEM {
    SYSCALL = 0x05,
    CLD     = 0xFC,
    STD     = 0xFD
};

enum STRING {
    STOSB = 0xAA,
    LODSB = 0xAC
};

enum EXTENTIONS {
    QWORD_OP = 0x48,
    EXT_REG  = 0x41,
    WIDE_OP  = 0x0F,
    REX_WRB  = 0x4D
};

enum JUMPS {
    CALL_NEAR = 0xE8,
    JMP_NEAR  = 0xE9,
    RET_NEAR  = 0xC3,

    JA_NEAR   = 0x87,
    JAE_NEAR  = 0x83,
    JB_NEAR   = 0x82,
    JBE_NEAR  = 0x86,
    JE_NEAR   = 0x84,
    JNE_NEAR  = 0x85,

    JG_NEAR   = 0x8F,
    JGE_NEAR  = 0x8D,
    JL_NEAR   = 0x8C,
    JLE_NEAR  = 0x8E,
};

enum UTILITY {
    PUSH_REG = 0x50,
    PUSH_NUM = 0x68,
    POP_REG  = 0x58,
    MOV_NUM  = 0xB8,
    MOV_REG  = 0x89,
    CMP_REG  = 0x39
};

enum MATH {
    ADD  = 0x01,
    SUB  = 0x29,
    IMUL = 0xAF,
    XOR  = 0x33,
};

#endif //X86_DEFINES_HPP
