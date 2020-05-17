#include <iostream>
#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "../simple_vector/vector.hpp"
#include "config/defines.hpp"

#ifdef DEBUG
    #define $DBG    if(1)
#else
    #define $DBG    if(0)
#endif

const char* Instruction::buf_begin = nullptr;
Vector<char*> Instruction::offsets;

const char* Instruction::IN_PTR = nullptr;
const char* Instruction::OUT_PTR = nullptr;
const char* Instruction::OUTC_PTR = nullptr;

void Instruction::set_buf_begin(const char* begin) {
    buf_begin = begin;
}


void Instruction::resize_offsets(const size_t& size) {
    offsets = std::move(Vector<char*>(size, nullptr));
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
        opcode(other.opcode) {
    $DBG std::cout << "moved instruction\n";
}


size_t Instruction::n_args() const {
    return arg.size();
}


void Instruction::set_addr(char* addr) const {

    if (!buf_begin) {
        throw std::runtime_error("Instruction::buf_begin is not set");
    } else if (static_cast<size_t>(opcode - buf_begin) >= offsets.size()) {
        throw std::runtime_error("Instruction::offsets is not resized properly");
    }

    offsets[opcode - buf_begin] = addr;
}


char* Instruction::write() const {

    char* dest = offsets[opcode - buf_begin];

    switch(*opcode) {

        case _END:
            dest = write_END(dest);
            break;

        case _ADD:
        case _SUB:
            dest = write_MATH(dest);
            break;

        case _MUL:
            dest = write_MUL(dest);
            break;

        case _DIV:
            dest = write_DIV(dest);
            break;

        case _POP:
            dest = write_POP(dest);
            break;

        case _IN:
            dest = write_IN(dest);
            break;

        case _OUT:
            dest = write_OUT(dest);
            break;

        case _OUTC:
            dest = write_OUTC(dest);
            break;

        case _PUSH:
            dest = write_PUSH(dest);
            break;

        case _MOV:
            dest = write_MOV(dest);
            break;

        case _JA:
        case _JAE:
        case _JB:
        case _JBE:
        case _JE:
        case _JNE:
            dest = write_JCOND(dest);
            break;

        case _JMP:
        case _CALL:
            dest = write_JMP(dest);
            break;

        case _RET:
            dest = write_RET(dest);
            break;

        case _SQRT:
            dest = write_SQRT(dest);
            break;

        default:
            throw std::runtime_error("not yet implemented opcode");
            break;
    }

    return dest;
}


void Instruction::fix_jmp() const {

    if (!IS_JMP[static_cast<size_t>(*opcode)]) {
        return;
    }

    char* addr = offsets[opcode - buf_begin];
    char op = *opcode;

    addr += ((op == _JMP || op == _CALL) ? 1 : 6);

    char* target_addr = offsets[arg[0]->val];

    if (!target_addr) {
        throw std::runtime_error("jmp address is not aligned");
    }

    int new_offset  = target_addr - (addr + sizeof(int));

    *reinterpret_cast<int*>(addr) = new_offset;
}
