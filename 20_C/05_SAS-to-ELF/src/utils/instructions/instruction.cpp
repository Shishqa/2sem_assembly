#include <iostream>
#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "../simple_vector/vector.hpp"
#include "config/defines.hpp"

Instruction::Instruction() :
        arg(Vector<const Argument*>()),
        jmp_target_idx(0),
        opcode(nullptr) {

    std::cout << "created empty instruction\n";
}

Instruction::Instruction(const char* op_ptr) {

    if (!op_ptr) {
        throw std::runtime_error("passing NULL to Instruction constructor");
    } else if (*op_ptr >= NUM_OF_COMMANDS) {
        throw std::runtime_error("passing not yet implemented opcode");
    }  

    opcode = op_ptr++;

    const size_t n_args = NUM_ARGS[static_cast<size_t>(*opcode)];

    arg.resize(n_args);
    for (size_t i = 0; i < n_args; ++i, op_ptr += sizeof(Argument)) {
        arg.push_back(reinterpret_cast<const Argument*>(op_ptr));
    }
}

Instruction::Instruction(Instruction&& other) :
        arg(std::move(other.arg)), 
        jmp_target_idx(other.jmp_target_idx),
        opcode(other.opcode) {
    std::cout << "moved instruction\n";
}

size_t Instruction::n_args() const {
    return arg.size();
}

size_t Instruction::write(char* dest) const {
    
    size_t buf_size = 0;

    switch(*opcode) {
        case 0:
            buf_size = write_END(dest);
            break;
        case 1:
        case 2:
            buf_size = write_MATH(dest);
            break;
        case 7:
            buf_size = write_POP(dest);
            break;
        case 10:
            buf_size = write_PUSH(dest);
            break;
        default:
            throw std::runtime_error("not yet implemented");
            break;
    }

    return buf_size;
}

