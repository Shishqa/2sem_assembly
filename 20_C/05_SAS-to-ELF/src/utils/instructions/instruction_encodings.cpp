#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/defines.hpp"

char* Instruction::write_END(char* dest) const {

    const byte_t END[10] = {MOV_NUM + RAX, 0x3C, 0x00, 0x00, 0x00, // mov rax, 0x3C 
                            QWORD_OP, XOR, Operand(3, RDI, RDI),   // xor rdi, rdi  
                            WIDE_OP, SYSCALL};                     // syscall

    memcpy(dest, END, sizeof(END));

    return dest + sizeof(END);
}


char* Instruction::write_MATH(char* dest) const {

    byte_t PRE[3]  = { POP_REG + RBX,           // pop rbx
                       POP_REG + RAX,           // pop rax
                       QWORD_OP       };        // *** rax, rbx
                                                // push rax    

    memcpy(dest, PRE, sizeof(PRE));
    dest += sizeof(PRE);

    switch(*opcode) {

        case _ADD:
            *dest = ADD;
            break;

        case _SUB:
            *dest = SUB;
            break;

        case _MUL:
            *dest = WIDE_OP; ++dest;
            *dest = IMUL;
            break;

        default:
            throw std::runtime_error("not yet implemented");
            break;
    }

    byte_t POST[2] = { Operand(3, RBX, RAX),
                       PUSH_REG + RAX        };

    memcpy(++dest, POST, sizeof(POST));             

    return dest + sizeof(POST);
}


char* Instruction::write_IN(char* dest) const {

    byte_t IN[] = { QWORD_OP, 0x83, 0xEC, 0x0B,                 // sub  rsp, 11
                    QWORD_OP, XOR, Operand(3, RAX, RAX),        // xor  rax, rax
                    QWORD_OP, MOV_REG, Operand(3, RSP, RSI),    // mov  rsi, rsp
                    QWORD_OP, XOR, Operand(3, RDI, RDI),        // xor  rdi, rdi
                    0xBA, 0x0A, 0x00, 0x00, 0x00,               // mov  edx, 10
                    0x41, 0x53,
                    0x0F, 0x05,                                 // syscall
                    0x41, 0x5B,
                    0xFC,                                       // cld
                    QWORD_OP, XOR, Operand(3, RDI, RDI),        // xor  rdi, rdi
                    QWORD_OP, XOR, Operand(3, RAX, RAX),        // xor  rax, rax
                    0xAC,                                       // lodsb
                    0x2C, 0x30,                                 // sub  al, '0'
                    0x3C, 0x09,                                 // cmp  al, 9
                    0x77, 0x09,                                 // ja   .end
                    QWORD_OP, 0x6B, 0xFF, 0x0A,                 // imul rdi, rdi, 10
                    QWORD_OP, 0x01, 0xC7,                       // add  rdi, rax
                    0xEB, 0xF0,                                 // jmp  .continue
                    0x48, 0x83, 0xC4, 0x0B,                     // sub  rsp, 0xb
                    0x57 };                                     // push rdi

    memcpy(dest, IN, sizeof(IN));

    return dest + sizeof(IN);
}


char* Instruction::write_OUT(char* dest) const {

    byte_t OUT[] = { POP_REG + RSI,                             // pop  rsi
                     QWORD_OP, 0x83, 0xEC, 0x0B,                // sub  rsp, 11
                     0x48, 0x89, 0xE7,                          // mov  rdi, rsp
                     0x48, 0x83, 0xC7, 0x0A,                    // add  rdi, 10
                     0xFD,                                      // std
                     MOV_NUM + RBX, 0x0A, 0x00, 0x00, 0x00,     // mov  ebx, 10
                     QWORD_OP, XOR, Operand(3, RCX, RCX),       // xor  rcx, rcx
                     0x31, 0xD2,                                // xor  edx, edx
                     0x89, 0xF0,                                // mov  eax, esi
                     0xF7, 0xF3,                                // div  ebx
                     0x89, 0xC6,                                // mov  esi, eax
                     0x88, 0xD0,                                // mov  al, dl
                     0x04, 0x30,                                // add  al, 0x30
                     0xAA,                                      // stosb
                     0x48, 0xFF, 0xC1,                          // inc  rcx
                     0x83, 0xFE, 0x00,                          // cmp  esi, 0
                     0x75, 0xEB,                                // jne  .continue
                     0x48, 0xFF, 0xC7,                          // inc  rdi
                     0xB8, 0x01, 0x00, 0x00, 0x00,              // mov  eax, 1
                     0x48, 0x89, 0xFE,                          // mov  rsi, rdi
                     0xBF, 0x01, 0x00, 0x00, 0x00,              // mov  edi, 1
                     0x48, 0x89, 0xCA,                          // mov  rdx, rcx
                     0x41, 0x53,
                     0x0F, 0x05,                                // syscall
                     0x41, 0x5B,
                     0x48, 0x83, 0xC4, 0x0B };                  // add  rsp, 11

    memcpy(dest, OUT, sizeof(OUT));

    return dest + sizeof(OUT);
}


char* Instruction::write_PUSH(char* dest) const {

    const Argument* arg1 = arg[0];

    if (arg1->int_param) {

        *dest = PUSH_NUM;
        memcpy(++dest, &arg1->val, sizeof(arg1->val));

        return dest + sizeof(arg1->val);

    } else if (arg1->reg_param) {

        CHECK_REG(arg1)

        *dest = EXT_REG;                ++dest; 
        *dest = PUSH_REG + arg1->reg;   ++dest;

        return dest;

    } else {
        throw std::runtime_error("not yet implemented push combination");
    }
}

char* Instruction::write_POP(char* dest) const {

    const Argument* arg1 = arg[0];

    if (arg1->reg_param && !arg1->mem_param && !arg1->int_param) {
        
        CHECK_REG(arg1)

        *dest = EXT_REG;                ++dest;
        *dest = POP_REG + arg1->reg;    ++dest;

        return dest;

    } else {
        throw std::runtime_error("not yet implemented");
    }
}

char* Instruction::write_MOV(char* dest) const {

    const Argument* arg1 = arg[0];
    const Argument* arg2 = arg[1];

    if (arg1->reg_param && arg2->reg_param) {
        
        *dest = REX_WRB;                            ++dest;
        *dest = MOV_REG;                            ++dest;
        *dest = Operand(3, arg1->reg, arg2->reg);   ++dest;

    } else if (arg1->int_param && arg2->reg_param) {

        *dest = REX_WRB;                            ++dest;
        *dest = XOR;                                ++dest;
        *dest = Operand(3, arg2->reg, arg2->reg);   ++dest;
        *dest = EXT_REG;                            ++dest;
        *dest = MOV_NUM + arg2->reg;                ++dest;
        *reinterpret_cast<int*>(dest) = arg1->val;  dest += sizeof(arg1->val);

    } else {
        throw std::runtime_error("not yet implemented");
    }

    return dest;
}

char* Instruction::write_JMP(char* dest) const {

    const Argument* arg1 = arg[0];

    switch (*opcode) {

        case _JMP:
            *dest = JMP_NEAR;
            break;

        case _CALL:
            *dest = CALL_NEAR;
            break;
    }

    ++dest;
    *reinterpret_cast<int*>(dest) = arg1->val; 

    return dest + sizeof(arg1->val);
}

char* Instruction::write_JCOND(char* dest) const {

    const Argument* arg1 = arg[0];

    *dest = POP_REG + RBX;          ++dest;
    *dest = POP_REG + RAX;          ++dest;
    *dest = QWORD_OP;               ++dest;
    *dest = CMP_REG;                ++dest;
    *dest = Operand(3, RAX, RBX);   ++dest;

    *dest = WIDE_OP;                ++dest;

    switch (*opcode) {
    
        case _JA:
            *dest = JA_NEAR;
            break;

        case _JAE:
            *dest = JAE_NEAR;
            break;

        case _JB:
            *dest = JB_NEAR;
            break;

        case _JBE:
            *dest = JBE_NEAR;
            break;

        case _JE:
            *dest = JE_NEAR;
            break;

        case _JNE:
            *dest = JNE_NEAR;
            break;

        default:
            throw std::runtime_error("not yet implemented");
            break;
    }

    ++dest;
    *reinterpret_cast<int*>(dest) = arg1->val;  

    return dest + sizeof(arg1->val);
}

char* Instruction::write_RET(char* dest) const {

    *dest = RET_NEAR;

    return dest + 1;
}
