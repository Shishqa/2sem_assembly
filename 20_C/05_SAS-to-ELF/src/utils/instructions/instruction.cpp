#include <cstring>
#include <stdexcept>

#include "instruction.hpp"

Instruction::Instruction(const char* op_ptr, const size_t& offset) :
        buf_size(0), arg_cnt(0), buffer(nullptr),
        opcode(0), is_active(true), offset(0) {

    if (!op_ptr) {
        throw std::runtime_error("passing NULL to Instruction constructor");
    }

    opcode = *op_ptr;
    ++op_ptr;

    switch(opcode) {
    case 0:
        encode_END();
        break;
    case 1:
    case 2:
    case 3:
        encode_ARITHMETICS();
        break;
    case 7:
        encode_POP(op_ptr);
        break;
    case 10:
        encode_PUSH(op_ptr);
        break;
    default:
        throw std::runtime_error("not yet implemented");
        break;
    }
}

Instruction::~Instruction() {
    delete[] buffer;
}

size_t Instruction::size() const {
    return (is_active ? buf_size : 0);
}

size_t Instruction::n_args() const {
    return (is_active ? arg_cnt : 0);
}

size_t Instruction::write(char* dest) const {
    memcpy(dest, buffer, buf_size);
    return buf_size;
}

