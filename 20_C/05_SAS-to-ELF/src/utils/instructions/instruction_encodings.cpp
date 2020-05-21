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

    byte_t OUTF[] = { 

        EXT_REG, PUSH_REG + R11,                    // push  r11 
        QWORD_OP, 0x83, 0xEC, 0x10,                 // sub   rsp, 16
                                                    //
        0x41, 0xBB, 0x01, 0x00, 0x00, 0x00,         // mov   r11d, 1
        0x83, 0xFE, 0x00,                           // cmp   rsi, 0
        0x7D, 0x05,                                 // jge   .continue >---*
        0x4D, 0x31, 0xDB,                           // xor   r11, r11      |
        0xF7, 0xDE,                                 // neg   rsi           |
                                                    //                     |
        PUSH_REG + RCX,                             // push  rcx <---------*
        MOV_NUM + RBX, 0x0A, 0x00, 0x00, 0x00,      // mov   ebx, 10          
        QWORD_OP, 0x8D, 0x7C, 0x24, 0x10,           // lea   rdi, [rsp + 16]
        STD,                                        // std
                                                    //
        XOR,  Operand(3, RDX, RDX),                 // xor   edx, edx <--*
        MOV_REG, Operand(3, RSI, RAX),              // mov   eax, esi    |
        0xF7, 0xF3,                                 // div   ebx         |
        MOV_REG, Operand(3, RAX, RSI),              // mov   esi, eax    |
        0x88, 0xD0,                                 // mov   al, dl      |
        0x04, 0x30,                                 // add   al, '0'     |
        STOSB,                                      // stosb             |
        QWORD_OP, 0xFF, 0xC8 + RCX,                 // dec   rcx         |
        0x75, 0xEE,                                 // jne   .convert >--*
                                                    //
        POP_REG + RCX,                              // pop   rcx
        QWORD_OP, 0xFF, 0xC0 + RCX,                 // inc   rcx
        0xB0 + RAX, 0x2E,                           // mov   al, '.'
        STOSB,                                      // stosb
                                                    //
        XOR,  Operand(3, RDX, RDX),                 // xor   edx, edx <--*
        MOV_REG, Operand(3, RSI, RAX),              // mov   eax, esi    |
        0xF7, 0xF3,                                 // div   ebx         |
        MOV_REG, Operand(3, RAX, RSI),              // mov   esi, eax    |
        0x88, 0xD0,                                 // mov   al, dl      |
        0x04, 0x30,                                 // add   al, '0'     |
        STOSB,                                      // stosb             |
        QWORD_OP, 0xFF, 0xC0 + RCX,                 // inc   rcx         |
        0x83, 0xFE, 0x00,                           // cmp   esi, 0      |
        0x75, 0xEB,                                 // jne   .convert >--*
                                                    //
        0x49, 0x83, 0xFB, 0x00,                     // cmp   r11, 0 
        0x75, 0x08,                                 // jne   .positive >-----*
        0xC6, 0x07, 0x2D,                           // mov   byte [rdi], '-' |
        0x48, 0xFF, 0xC0 + RCX,                     // inc   rcx             |
        0xEB, 0x03,                                 // jmp   .print >--------+--*
        0x48, 0xFF, 0xC0 + RDI,                     // inc   rdi <-----------*  |
                                                    //                          |
        MOV_NUM + RAX, 0x01, 0x00, 0x00, 0x00,      // mov   eax, 1   ;write <--*
        QWORD_OP, MOV_REG, Operand(3, RDI, RSI),    // mov   rsi, rdi ;from [rdi]
        MOV_NUM + RDI, 0x01, 0x00, 0x00, 0x00,      // mov   edi, 1   ;to STDOUT
        QWORD_OP, MOV_REG, Operand(3, RCX, RDX),    // mov   rdx, rcx ;RCX symbols
        WIDE_OP, SYSCALL,                           // syscall
                                                    //
        QWORD_OP, 0x83, 0xC4, 0x10,                 // add   rsp, 16
        EXT_REG, POP_REG + R11,                     // pop   r11
        RET_NEAR                                    // ret
    };             

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

    const byte_t END[10] = {MOV_NUM + RAX, 0x3C, 0x00, 0x00, 0x00, // mov rax, 0x3C 
                            QWORD_OP, XOR, Operand(3, RDI, RDI),   // xor rdi, rdi  
                            WIDE_OP, SYSCALL};                     // syscall

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
