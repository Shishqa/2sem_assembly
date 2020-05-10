#ifndef DEFINES_HPP
#define DEFINES_HPP

#define SET_BUF( size )     \
    buf_size = size;        \
    buffer = new char[size];

#define CHECK_REG( argument )                                   \
    if (argument.reg >= 8) {                                    \
        throw std::runtime_error("unsupported register used");  \
    }

typedef unsigned char byte_t;

static const byte_t EXT      = 0x41;

static const byte_t PUSH_NUM = 0x68;
static const byte_t PUSH_REG = 0x50;

static const byte_t POP_REG  = 0x58;

#endif //DEFINES_HPP
