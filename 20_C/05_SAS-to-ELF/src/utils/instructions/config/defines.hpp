#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <cstdio>

#define SET_BUF( size )     \
    buf_size = size;        \
    buffer = new char[size];

#define CHECK_REG( argument )                                   \
    if (argument->reg >= 8) {                                    \
        throw std::runtime_error("unsupported register used");  \
    }

struct Operand {
    volatile char r_m : 3;
    volatile char reg : 3;
    volatile char mod : 2;

    Operand(const char& _mod, const char& _reg, const char& _r_m) {
        mod = _mod;
        reg = _reg;
        r_m = _r_m;
    };

    operator char() {
        return *reinterpret_cast<char*>(this);
    }
};

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

enum EXTENTIONS {
    QWORD_OP = 0x48,
    EXT_REG  = 0x41
};

enum UTILITY {
    PUSH_REG = 0x50,
    PUSH_NUM = 0x68,
    POP_REG  = 0x58,
    MOV_NUM  = 0xB8
};

enum MATH {
    ADD  = 0x01,
    SUB  = 0x29,
    IMUL = 0xF7,
    XOR  = 0x33
};

static const size_t NUM_OF_COMMANDS = 11;

static const size_t NUM_ARGS[NUM_OF_COMMANDS] = {
    0, // END
    0, // ADD
    0, // SUB
    0, // MUL
    0, // DIV
    0, //
    0,
    1, // POP
    0, // 
    0, //
    1  // PUSH
};

typedef unsigned char byte_t;

#endif //DEFINES_HPP
