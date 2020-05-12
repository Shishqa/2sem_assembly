#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/defines.hpp"

size_t Instruction::write_END(char* dest) const {

    const byte_t END[10] = {MOV_NUM + RAX, 0x3C, 0x00, 0x00, 0x00, 
                            QWORD_OP, XOR, Operand(3, RDI, RDI),  
                            0x0F, 0x05};                  // syscall

    memcpy(dest, END, sizeof(END));

    return sizeof(END);
}


size_t Instruction::write_MATH(char* dest) const {

    byte_t OP[6] = { POP_REG + RAX,                // pop  rcx
                     POP_REG + RBX,                // pop  rax
                     QWORD_OP, 0x00, Operand(3, RBX, RAX),    // ???  rax, rcx
                     PUSH_REG + RAX };              // push rax

    char op = *opcode;

    if (op == 1) {
        OP[3] = 0x01;
    } else if (op == 2) {
        OP[3] = 0x29;
    } 
    /*
    else if (op == 3) {
        OP[3] = 0xF7;
        OP[4] = 0xE9;
    } else if (op == 4) {
        OP[0] = 0x90;
    }*/

    memcpy(dest, OP, sizeof(OP));

    return sizeof(OP);
}

size_t Instruction::write_PUSH(char* dest) const {

    const Argument* arg1 = arg[0];

    if (arg1->int_param) {

        *dest = PUSH_NUM;
        memcpy(dest + 1, &arg1->val, sizeof(arg1->val));

        return 1 + sizeof(arg1->val);

    } else if (arg1->reg_param) {

        CHECK_REG(arg1)

        dest[0] = EXT_REG;
        dest[1] = PUSH_REG + arg1->reg;

        return 2;

    } else {
        throw std::runtime_error("not yet implemented push combination");
    }
}

size_t Instruction::write_POP(char* dest) const {

    const Argument* arg1 = arg[0];

    if (arg1->reg_param && !arg1->mem_param && !arg1->int_param) {
        
        CHECK_REG(arg1)

        dest[0] = EXT_REG;
        dest[1] = POP_REG + arg1->reg;

        return 2;

    } else {
        throw std::runtime_error("not yet implemented");
    }
}



