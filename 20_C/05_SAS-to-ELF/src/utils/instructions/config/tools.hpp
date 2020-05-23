#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <cstdio>

#define CHECK_REG( argument )                                   \
    if (argument->reg >= 8) {                                   \
        throw std::runtime_error("unsupported register used");  \
    }

struct OP {
    volatile char r_m : 3;
    volatile char reg : 3;
    volatile char mod : 2;

    OP(const char& _mod, const char& _reg, const char& _r_m) {
        mod = _mod;
        reg = _reg;
        r_m = _r_m;
    };

    operator unsigned char() {
        return *reinterpret_cast<char*>(this);
    }
};

typedef unsigned char byte_t;

#include "sas_instructions.hpp"

#endif
