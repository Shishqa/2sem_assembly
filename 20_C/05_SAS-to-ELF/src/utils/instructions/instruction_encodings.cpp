#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/standard_implementations.hpp"

char* Instruction::write_preamble(char* dest) {
    
    *dest = JMP_NEAR; ++dest;
    *reinterpret_cast<int*>(dest) = sizeof(IN) + 
                                    sizeof(OUT) + 
                                    sizeof(OUTC) + 
                                    sizeof(OUTF);
    dest += sizeof(int);

    IN_PTR = dest;
    memcpy(dest, IN, sizeof(IN));
    dest += sizeof(IN);

    OUT_PTR = dest;
    memcpy(dest, OUT, sizeof(OUT));
    dest += sizeof(OUT);

    OUTC_PTR = dest;
    memcpy(dest, OUTC, sizeof(OUTC));
    dest += sizeof(OUTC);

    OUTF_PTR = dest;
    memcpy(dest, OUTF, sizeof(OUTF));
    return dest + sizeof(OUTF);
}


char* Instruction::write_END(char* dest) const {

    memcpy(dest, END, sizeof(END));
    return dest + sizeof(END);
}


char* Instruction::write_MATH(char* dest) const {

    byte_t OP[] = { 
        POP_REG + RBX,                  // pop      rbx
        POP_REG + RAX,                  // pop      rax
        0x00, Operand(3, RBX, RAX),     // add/sub  eax, ebx
        PUSH_REG + RAX                  // push     rax
    };        
    static const size_t OP_OFFSET = 2;

    switch(*opcode) {

        case _ADD:
            OP[OP_OFFSET] = ADD;
            break;

        case _SUB:
            OP[OP_OFFSET] = SUB;
            break;

        default:
            throw std::runtime_error("not yet implemented");
            break;
    }

    memcpy(dest, OP, sizeof(OP));             

    return dest + sizeof(OP);
}


char* Instruction::write_MUL(char* dest) const {

    byte_t MUL[] = {
        POP_REG + RBX,                          // pop  rbx
        POP_REG + RAX,                          // pop  rax
        WIDE_OP, IMUL, Operand(3, RAX, RBX),    // imul eax, ebx
        PUSH_REG + RAX                          // push rax
    };

    memcpy(dest, MUL, sizeof(MUL));

    return dest + sizeof(MUL);
}


char* Instruction::write_DIV(char* dest) const {

    byte_t DIV[] = {
        POP_REG + RBX,                          // pop  rbx
        POP_REG + RAX,                          // pop  rax
        XOR, Operand(3, RDX, RDX),              // xor  edx, edx
        0x83, 0xF8, 0x00,                       // cmp  eax, 0
        0x7D, 0x05,                             // jge  .div >----------------*
        MOV_NUM + RDX, 0xFF, 0xFF, 0xFF, 0xFF,  // mov  edx, 0xFFFFFFFF ; -1  |
        0xF7, 0xF8 + RBX,                       // idiv ebx <-----------------*
        PUSH_REG + RAX                          // push rax
    };

    memcpy(dest, DIV, sizeof(DIV));

    return dest + sizeof(DIV);
}


char* Instruction::write_IN(char* dest) const {

    *dest = CALL_NEAR; ++dest;

    *reinterpret_cast<int*>(dest) = IN_PTR - (dest + sizeof(int));
    dest += sizeof(int);

    *dest = PUSH_REG + RDI; ++dest;

    return dest;
}


char* Instruction::write_OUT(char* dest) const {

    *dest = POP_REG + RSI; ++dest;

    *dest = CALL_NEAR;  ++dest;
    *reinterpret_cast<int*>(dest) = OUT_PTR - (dest + sizeof(int));

    return dest + sizeof(int);
}


char* Instruction::write_OUTC(char* dest) const {

    *dest = POP_REG + RSI; ++dest;

    *dest = CALL_NEAR;  ++dest;
    *reinterpret_cast<int*>(dest) = OUTC_PTR - (dest + sizeof(int));

    return dest + sizeof(int);
}


char* Instruction::write_OUTF(char* dest) const {

    *dest = POP_REG + RSI;                          ++dest;
    *dest = MOV_NUM + RCX;                          ++dest;
    *reinterpret_cast<int*>(dest) = arg[0]->val;    dest += sizeof(int);

    *dest = CALL_NEAR;                              ++dest;

    *reinterpret_cast<int*>(dest) = OUTF_PTR - (dest + sizeof(int));
    return dest + sizeof(int);
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

    byte_t JMP[] = {
        POP_REG + RBX,                          // pop rax
        POP_REG + RAX,                          // pop rbx
        CMP_REG, Operand(3, RBX, RAX),          // cmp eax, ebx
        WIDE_OP, 0x00, 0xFF, 0xFF, 0xFF, 0xFF   // j* {val}
    };
    unsigned char* j_op = JMP + 5;

    *reinterpret_cast<int*>(JMP + 6) = arg1->val;

    switch (*opcode) {
    
        case _JA:
            *j_op = JG_NEAR;
            break;

        case _JAE:
            *j_op = JGE_NEAR;
            break;

        case _JB:
            *j_op = JL_NEAR;
            break;

        case _JBE:
            *j_op = JLE_NEAR;
            break;

        case _JE:
            *j_op = JE_NEAR;
            break;

        case _JNE:
            *j_op = JNE_NEAR;
            break;

        default:
            throw std::runtime_error("not yet implemented");
            break;
    }
    
    memcpy(dest, JMP, sizeof(JMP));

    return dest + sizeof(JMP);
}

char* Instruction::write_RET(char* dest) const {

    *dest = RET_NEAR; // ret

    return dest + 1;
}

char* Instruction::write_SQRT(char* dest) const {

    byte_t SQRT[] = {
        POP_REG + RAX,              // pop       rax
        0xF2, 0x0F, 0x2A, 0xC0,     // cvtsi2sd  xmm0, eax
        0xF2, 0x0F, 0x51, 0xC0,     // sqrtsd    xmm0, xmm0
        0xF2, 0x0F, 0x2C, 0xC0,     // cvttsd2si eax, xmm0
        PUSH_REG + RAX              // push      rax
    };

    memcpy(dest, SQRT, sizeof(SQRT));

    return dest + sizeof(SQRT);
}
