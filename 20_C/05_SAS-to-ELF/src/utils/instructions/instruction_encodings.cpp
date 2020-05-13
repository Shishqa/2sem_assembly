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

    const int TEST_VAL = 1;

    *dest = PUSH_NUM;
    memcpy(++dest, &TEST_VAL, sizeof(TEST_VAL));

    return dest + sizeof(TEST_VAL);
}


char* Instruction::write_OUT(char* dest) const {

    *dest = POP_REG + RDI;

    return dest + 1;
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
