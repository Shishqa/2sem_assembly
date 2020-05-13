#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/defines.hpp"

char* Instruction::write_END(char* dest) const {

    const byte_t END[10] = {MOV_NUM + RAX, 0x3C, 0x00, 0x00, 0x00, 
                            QWORD_OP, XOR, Operand(3, RDI, RDI),  
                            WIDE_OP, SYSCALL};

    memcpy(dest, END, sizeof(END));

    return dest + sizeof(END);
}


char* Instruction::write_MATH(char* dest) const {

    byte_t PRE[3]  = { POP_REG + RBX,
                       POP_REG + RAX,
                       QWORD_OP       };

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

    byte_t IN[48] = {QWORD_OP, 0x83, 0xEC, 0x0A,
                   QWORD_OP, XOR, Operand(3, RAX, RAX),
                   QWORD_OP, XOR, Operand(3, RDI, RDI),
                   MOV_NUM + RDX, 0x0A, 0x00, 0x00, 0x00,
                   QWORD_OP, MOV_REG, Operand(3, RSP, RSI),
                   WIDE_OP, SYSCALL,
                   0xAC,                // lodsb
                   0x3C, 0x30,
                   0x72, 0x12,
                   0x3C, 0x39,
                   0x77, 0x0E,
                   0x2C, 0x30,
                   QWORD_OP, 0x6B, Operand(3, RDI, RDI), 0x0A,
                   QWORD_OP, ADD, Operand(3, RAX, RDI),
                   QWORD_OP, 0xFF, 0xCA,
                   0x73, 0xE9,
                   QWORD_OP, 0x83, 0xC4, 0x0A,
                   PUSH_REG + RDI};

    memcpy(dest, IN, sizeof(IN));

    return dest + sizeof(IN);
}


char* Instruction::write_OUT(char* dest) const {

    byte_t OUT[62] = { 0x59,
                     0x48, 0x89, 0xE7,
                     0x48, 0x83, 0xEC, 0x0B,
                     0xBB, 0x0A, 0x00, 0x00, 0x00,
                     0xFD,
                     0xC6, 0x07, 0x00,
                     0x48, 0xFF, 0xCF,
                     0x31, 0xD2,
                     0x89, 0xC8, 
                     0xF7, 0xF3,
                     0x89, 0xC1,
                     0x88, 0xD0,
                     0x04, 0x30,
                     0xAA,
                     0x83, 0xF9, 0x00,
                     0x75, 0xEE,
                     0xB8, 0x01, 0x00, 0x00, 0x00,
                     0x48, 0x89, 0xFE,
                     0xBF, 0x01, 0x00, 0x00, 0x00,
                     0xBA, 0x0A, 0x00, 0x00, 0x00,
                     0x0F, 0x05,
                     0x48, 0x83, 0xC4, 0x0B };

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
    *dest = Operand(3, RBX, RAX);   ++dest;

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
