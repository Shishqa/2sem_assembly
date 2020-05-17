#include <cstring>
#include <stdexcept>

#include "instruction.hpp"
#include "argument.hpp"

#include "config/defines.hpp"

char* Instruction::write_preamble(char* dest) {
    
    byte_t IN[] = { 
        EXT_REG, PUSH_REG + R11,                    // push  r11
        QWORD_OP, 0x83, 0xEC, 0x0B,                 // sub   rsp, 11
                                                    //
        QWORD_OP, XOR, Operand(3, RAX, RAX),        // xor   rax, rax ;read
        QWORD_OP, MOV_REG, Operand(3, RSP, RSI),    // mov   rsi, rsp ;to [rsp]
        QWORD_OP, XOR, Operand(3, RDI, RDI),        // xor   rdi, rdi ;from STDIN
        MOV_NUM + RDX, 0x0A, 0x00, 0x00, 0x00,      // mov   edx, 10  ;10 symbols
        WIDE_OP, SYSCALL,                           // syscall
                                                    //
        EXT_REG, MOV_NUM + R11,0x01,0x00,0x00,0x00, // mov   r11d, 1
        0x80, 0x3E, 0x2D,                           // cmp   byte [rsi], '-'
        0x75, 0x0A,                                 // jne   .proceed >-----*
        0x49, 0xC7, 0xC3, 0xFF, 0xFF, 0xFF, 0xFF,   // mov   r11, 0xff..ff  | 
        QWORD_OP, 0xFF, 0xC6,                       // inc   rsi            |
                                                    //                      |
        CLD,                                        // cld <----------------*
        QWORD_OP, XOR, Operand(3, RDI, RDI),        // xor   rdi, rdi
        QWORD_OP, XOR, Operand(3, RAX, RAX),        // xor   rax, rax
                                                    //
        LODSB,                                      // lodsb <------------*
        0x2C, 0x30,                                 // sub   al, '0'      |
        0x3C, 0x09,                                 // cmp   al,  9       |
        0x77, 0x09,                                 // ja    .end >-------+--*
        QWORD_OP, 0x6B, 0xFF, 0x0A,                 // imul  rdi, rdi, 10 |  |
        QWORD_OP, ADD, Operand(3, RAX, RDI),        // add   rdi, rax     |  |
        0xEB, 0xF0,                                 // jmp   .continue >--*  |
                                                    //                       |
        QWORD_OP, 0x83, 0xC4, 0x0B,                 // sub   rsp, 0xb <------*
        0x49, 0x0F, 0xAF, 0xFB,                     // imul  rdi, r11
        0x41, 0x5B,                                 // pop   r11
        RET_NEAR                                    // ret
    };

    byte_t OUT[] = { 

        EXT_REG, PUSH_REG + R11,                    // push  r11 
        QWORD_OP, 0x83, 0xEC, 0x0B,                 // sub   rsp, 11
                                                    //
        0x41, 0xBB, 0x01, 0x00, 0x00, 0x00,         // mov   r11d, 1
        0x83, 0xFE, 0x00,                           // cmp   rsi, 0
        0x7D, 0x05,                                 // jge   .continue >--*
        0x4D, 0x31, 0xDB,                           // xor   r11, r11     |
        0xF7, 0xDE,                                 // neg   rsi          |
                                                    //                    |
        QWORD_OP, MOV_REG, Operand(3, RSP, RDI),    // mov   rdi, rsp <---*
        QWORD_OP, 0x83, 0xC7, 0x0A,                 // add   rdi, 10
        STD,                                        // std
        MOV_NUM + RBX, 0x0A, 0x00, 0x00, 0x00,      // mov   ebx, 10
        QWORD_OP, XOR, Operand(3, RCX, RCX),        // xor   rcx, rcx
                                                    //
        XOR,  Operand(3, RDX, RDX),                 // xor   edx, edx <--*
        MOV_REG, Operand(3, RSI, RAX),              // mov   eax, esi    |
        0xF7, 0xF3,                                 // div   ebx         |
        MOV_REG, Operand(3, RAX, RSI),              // mov   esi, eax    |
        0x88, 0xD0,                                 // mov   al, dl      |
        0x04, 0x30,                                 // add   al, '0'     |
        STOSB,                                      // stosb             |
        QWORD_OP, 0xFF, 0xC1,                       // inc   rcx         |
        0x83, 0xFE, 0x00,                           // cmp   esi, 0      |
        0x75, 0xEB,                                 // jne   .convert >--*
                                                    //
        0x49, 0x83, 0xFB, 0x00,                     // cmp   r11, 0 
        0x75, 0x08,                                 // jne   .positive >-----*
        0xC6, 0x07, 0x2D,                           // mov   byte [rdi], '-' |
        0x48, 0xFF, 0xC1,                           // inc   rcx             |
        0xEB, 0x03,                                 // jmp   .print >--------+--*
        0x48, 0xFF, 0xC7,                           // inc   rdi <-----------*  |
                                                    //                          |
        MOV_NUM + RAX, 0x01, 0x00, 0x00, 0x00,      // mov   eax, 1   ;write <--*
        QWORD_OP, MOV_REG, Operand(3, RDI, RSI),    // mov   rsi, rdi ;from [rdi]
        MOV_NUM + RDI, 0x01, 0x00, 0x00, 0x00,      // mov   edi, 1   ;to STDOUT
        QWORD_OP, MOV_REG, Operand(3, RCX, RDX),    // mov   rdx, rcx ;RCX symbols
        WIDE_OP, SYSCALL,                           // syscall
                                                    //
        QWORD_OP, 0x83, 0xC4, 0x0B,                 // add   rsp, 11
        EXT_REG, POP_REG + R11,                     // pop   r11
        RET_NEAR                                    // ret
    };             

    byte_t OUTC[] = {

        EXT_REG, PUSH_REG + R11,                    // push r11
        QWORD_OP, 0xFF, 0xCC,                       // dec  rsp
                                                    //
        0x40, 0x88, 0x34, 0x24,                     // mov  byte [rsp], sil
                                                    //
        MOV_NUM + RAX, 0x01, 0x00, 0x00, 0x00,      // mov  eax, 1   ;write
        QWORD_OP, MOV_REG, Operand(3, RSP, RSI),    // mov  rsi, rsp ;from [rsp]
        MOV_NUM + RDI, 0x01, 0x00, 0x00, 0x00,      // mov  edi, 1   ;to STDOUT
        MOV_NUM + RDX, 0x01, 0x00, 0x00, 0x00,      // mov  edx, 1   ;1 symbol
        WIDE_OP, SYSCALL,                           // syscall
                                                    //
        QWORD_OP, 0xFF, 0xC4,                       // inc  rsp
        EXT_REG, POP_REG + R11,                     // pop  r11
        RET_NEAR                                    // ret
    };
    
    *dest = JMP_NEAR; ++dest;
    *reinterpret_cast<int*>(dest) = sizeof(IN) + 
                                    sizeof(OUT) + 
                                    sizeof(OUTC);
    dest += sizeof(int);

    IN_PTR = dest;
    memcpy(dest, IN, sizeof(IN));
    dest += sizeof(IN);

    OUT_PTR = dest;
    memcpy(dest, OUT, sizeof(OUT));
    dest += sizeof(OUT);

    OUTC_PTR = dest;
    memcpy(dest, OUTC, sizeof(OUTC));

    return dest + sizeof(OUTC);
}


char* Instruction::write_END(char* dest) const {

    const byte_t END[10] = {MOV_NUM + RAX, 0x3C, 0x00, 0x00, 0x00, // mov rax, 0x3C 
                            QWORD_OP, XOR, Operand(3, RDI, RDI),   // xor rdi, rdi  
                            WIDE_OP, SYSCALL};                     // syscall

    memcpy(dest, END, sizeof(END));

    return dest + sizeof(END);
}


char* Instruction::write_MATH(char* dest) const {

    byte_t PRE[] = { 
        POP_REG + RBX,           // pop rbx
        POP_REG + RAX,           // pop rax
    };                                         

    memcpy(dest, PRE, sizeof(PRE));
    dest += sizeof(PRE);

    switch(*opcode) {

        case _ADD:
            *dest = ADD;
            break;

        case _SUB:
            *dest = SUB;
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


char* Instruction::write_MUL(char* dest) const {

    byte_t MUL[] = {
        POP_REG + RBX,
        POP_REG + RAX,
        WIDE_OP, IMUL, Operand(3, RAX, RBX),
        PUSH_REG + RAX
    };

    memcpy(dest, MUL, sizeof(MUL));

    return dest + sizeof(MUL);
}


char* Instruction::write_DIV(char* dest) const {

    byte_t DIV[] = {
        POP_REG + RBX,
        POP_REG + RAX,
        XOR, Operand(3, RDX, RDX),
        0x83, 0xF8, 0x00,
        0x7D, 0x05,
        0xBA, 0xFF, 0xFF, 0xFF, 0xFF,
        0xF7, 0xF8 + RBX,
        PUSH_REG + RAX
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
    *dest = CMP_REG;                ++dest;
    *dest = Operand(3, RBX, RAX);   ++dest;

    *dest = WIDE_OP;                ++dest;

    switch (*opcode) {
    
        case _JA:
            *dest = JG_NEAR;
            break;

        case _JAE:
            *dest = JGE_NEAR;
            break;

        case _JB:
            *dest = JL_NEAR;
            break;

        case _JBE:
            *dest = JLE_NEAR;
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

char* Instruction::write_SQRT(char* dest) const {

    byte_t SQRT[] = {
        POP_REG + RAX,
        0xF2, 0x0F, 0x2A, 0xC0,
        0xF2, 0x0F, 0x51, 0xC0,
        0xF2, 0x0F, 0x2C, 0xC0,
        PUSH_REG + RAX
    };

    memcpy(dest, SQRT, sizeof(SQRT));

    return dest + sizeof(SQRT);
}
