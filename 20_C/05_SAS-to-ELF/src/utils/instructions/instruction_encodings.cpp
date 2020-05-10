#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/defines.hpp"

void Instruction::encode_END() {

    arg_cnt = 0;

    const byte_t END[10] = {0xB8, 0x3C, 0x00, 0x00, 0x00, // mov rax, 0x3c
                            0x48, 0x31, 0xFF,             // xor rdi, rdi
                            0x0F, 0x05};                  // syscall

    SET_BUF(sizeof(END))

    memcpy(buffer, END, buf_size);
}

void Instruction::encode_PUSH(const char* arg_ptr) {

    arg_cnt = 1;
    Argument arg(arg_ptr);

    if (arg.int_param) {
        
        SET_BUF(5)

        buffer[0] = PUSH_NUM;
        memcpy(buffer + 1, &arg.val, sizeof(arg.val));

    } else if (arg.reg_param) {

        CHECK_REG(arg)
        SET_BUF(2)

        buffer[0] = EXT;
        buffer[1] = PUSH_REG + arg.reg;

    } else {
        throw std::runtime_error("not yet implemented push combination");
    }
}

void Instruction::encode_POP(const char* arg_ptr) {

    arg_cnt = 1;
    Argument arg(arg_ptr);

    if (arg.reg_param && !arg.mem_param && !arg.int_param) {
        
        CHECK_REG(arg)
        SET_BUF(2)

        buffer[0] = EXT;
        buffer[1] = POP_REG + arg.reg;

    } else {
        throw std::runtime_error("not yet implemented");
    }
}

void Instruction::encode_ARITHMETICS() {

    arg_cnt = 0;

    byte_t OP[6] = { 0x59,                // pop  rcx
                     0x58,                // pop  rax
                     0x48, 0x00, 0xC8,    // ???  rax, rcx
                     0x50 };              // push rax

    if (opcode == 1) {
        OP[3] = 0x01;
    } else if (opcode == 2) {
        OP[3] = 0x29;
    } else if (opcode == 3) {
        OP[3] = 0xF7;
        OP[4] = 0xE9;
    }

    SET_BUF(sizeof(OP))

    memcpy(buffer, OP, sizeof(OP));
}
