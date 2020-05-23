#ifndef STANDARD_IMPLEMENTATIONS_HPP
#define STANDARD_IMPLEMENTATIONS_HPP

#include "tools.hpp"
#include "x86_defines.hpp"

static const byte_t COMPARE[] = {
    POP_REG + RBX,              // pop  rbx
    POP_REG + RAX,              // pop  rax
    CMP_REG, OP(3, RBX, RAX)    // cmp  eax, ebx
};

static const byte_t END[] = {
    MOV_NUM + RAX, 0x3C,0x00,0x00,0x00,   // mov   rax, 0x3C ; exit
    REX_W, XOR, OP(3, RDI, RDI),          // xor   rdi, rdi  ; 0
    WIDE_OP, SYSCALL                      // syscall
};                     

static const byte_t IN[] = {

    // IN:    -
    // DESTR: RAX RBX RDX RSI RDI CC

    REX_B, PUSH_REG + R11,                      // push  r11
    REX_W, 0x83, 0xEC, 0x10,                    // sub   rsp, 16
                                                //
    REX_W, XOR, OP(3, RAX, RAX),                // xor   rax, rax ;read
    REX_W, MOV_REG, OP(3, RSP, RSI),            // mov   rsi, rsp ;to [rsp]
    REX_W, XOR, OP(3, RDI, RDI),                // xor   rdi, rdi ;from STDIN
    MOV_NUM + RDX, 0x0A,0x00,0x00,0x00,         // mov   edx, 10  ;10 symbols
    WIDE_OP, SYSCALL,                           // syscall
                                                //
    REX_B, MOV_NUM + R11, 0x01,0x00,0x00,0x00,  // mov   r11d, 1
    0x80, 0x3E, 0x2D,                           // cmp   byte [rsi], '-'
    0x75, 0x0A,                                 // jne   .proceed >-----*
    REX_WB, 0xC7, 0xC3, 0xFF,0xFF,0xFF,0xFF,    // mov   r11, 0xff..ff  | 
    REX_W, 0xFF, INC + RSI,                     // inc   rsi            |
                                                //                      |
    CLD,                                        // cld <----------------*
    REX_W, XOR, OP(3, RDI, RDI),                // xor   rdi, rdi
    REX_W, XOR, OP(3, RAX, RAX),                // xor   rax, rax
                                                //
    LODSB,                                      // lodsb <------------*
    0x2C, 0x30,                                 // sub   al, '0'      |
    0x3C, 0x09,                                 // cmp   al,  9       |
    0x77, 0x09,                                 // ja    .end >-------+--*
    REX_W, 0x6B, 0xFF, 0x0A,                    // imul  rdi, rdi, 10 |  |
    REX_W, ADD, OP(3, RAX, RDI),                // add   rdi, rax     |  |
    0xEB, 0xF0,                                 // jmp   .continue >--*  |
                                                //                       |
    REX_W, 0x83, 0xC4, 0x10,                    // add   rsp, 16 <-------*
    REX_WB, WIDE_OP, IMUL, OP(3, RDI, R11),     // imul  rdi, r11
    REX_B, POP_REG + R11,                       // pop   r11
    RET_NEAR                                    // ret
};


static const byte_t OUT[] = { 

    // IN:    RSI <- signed int number to print
    // DESTR: RAX RBX RCX RDX RSI RDI CC

    REX_B, PUSH_REG + R11,                      // push  r11 
    REX_W, 0x83, 0xEC, 0x10,                    // sub   rsp, 16
                                                //
    REX_B, MOV_NUM + R11, 0x01,0x00,0x00,0x00,  // mov   r11d, 1
    0x83, 0xFE, 0x00,                           // cmp   rsi, 0
    0x7D, 0x05,                                 // jge   .continue >--*
    REX_WRB, XOR, OP(3, R11, R11),              // xor   r11, r11     |
    0xF7, 0xDE,                                 // neg   rsi          |
                                                //                    |
    REX_W, MOV_REG, OP(3, RSP, RDI),            // mov   rdi, rsp <---*
    REX_W, 0x83, 0xC7, 0x0A,                    // add   rdi, 10
    STD,                                        // std
    MOV_NUM + RBX, 0x0A,0x00,0x00,0x00,         // mov   ebx, 10
    REX_W, XOR, OP(3, RCX, RCX),                // xor   rcx, rcx
                                                //
    XOR, OP(3, RDX, RDX),                       // xor   edx, edx <--*
    MOV_REG, OP(3, RSI, RAX),                   // mov   eax, esi    |
    0xF7, 0xF3,                                 // div   ebx         |
    MOV_REG, OP(3, RAX, RSI),                   // mov   esi, eax    |
    0x88, 0xD0,                                 // mov   al, dl      |
    0x04, 0x30,                                 // add   al, '0'     |
    STOSB,                                      // stosb             |
    REX_W, 0xFF, INC + RCX,                     // inc   rcx         |
    0x83, 0xFE, 0x00,                           // cmp   esi, 0      |
    0x75, 0xEB,                                 // jne   .convert >--*
                                                //
    REX_WB, 0x83, 0xFB, 0x00,                   // cmp   r11, 0 
    0x75, 0x08,                                 // jne   .positive >-----*
    0xC6, 0x07, 0x2D,                           // mov   byte [rdi], '-' |
    REX_W, 0xFF, INC + RCX,                     // inc   rcx             |
    0xEB, 0x03,                                 // jmp   .print >--------+--*
    REX_W, 0xFF, INC + RDI,                     // inc   rdi <-----------*  |
                                                //                          |
    MOV_NUM + RAX, 0x01,0x00,0x00,0x00,         // mov   eax, 1   ;write <--*
    REX_W, MOV_REG, OP(3, RDI, RSI),            // mov   rsi, rdi ;from [rdi]
    MOV_NUM + RDI, 0x01,0x00,0x00,0x00,         // mov   edi, 1   ;to STDOUT
    REX_W, MOV_REG, OP(3, RCX, RDX),            // mov   rdx, rcx ;RCX symbols
    WIDE_OP, SYSCALL,                           // syscall
                                                //
    REX_W, 0x83, 0xC4, 0x10,                    // add   rsp, 16
    REX_B, POP_REG + R11,                       // pop   r11
    RET_NEAR                                    // ret
};             

static const byte_t OUTC[] = {

    // IN:    SIL <- ASCII symbol to print
    // DESTR: RAX RDX RSI RDI

    REX_B, PUSH_REG + R11,                      // push r11
    REX_W, 0xFF, DEC + RSP,                     // dec  rsp
                                                //
    REX, 0x88, 0x34, 0x24,                      // mov  byte [rsp], sil
                                                //
    MOV_NUM + RAX, 0x01,0x00,0x00,0x00,         // mov  eax, 1   ;write
    REX_W, MOV_REG, OP(3, RSP, RSI),            // mov  rsi, rsp ;from [rsp]
    MOV_NUM + RDI, 0x01,0x00,0x00,0x00,         // mov  edi, 1   ;to STDOUT
    MOV_NUM + RDX, 0x01,0x00,0x00,0x00,         // mov  edx, 1   ;1 symbol
    WIDE_OP, SYSCALL,                           // syscall
                                                //
    REX_W, 0xFF, INC + RSP,                     // inc  rsp
    REX_B, POP_REG + R11,                       // pop  r11
    RET_NEAR                                    // ret
};

static const byte_t OUTF[] = { 

    // IN:    RCX <- accuracy
    //        RSI <- int value multiplied by 10^acc
    // DESTR: RAX RBX RCX RDX RSI RDI CC

    REX_B, PUSH_REG + R11,                      // push  r11 
    REX_W, 0x83, 0xEC, 0x10,                    // sub   rsp, 16
                                                //
    REX_B, MOV_NUM + R11, 0x01,0x00,0x00,0x00,  // mov   r11d, 1
    0x83, 0xFE, 0x00,                           // cmp   rsi, 0
    0x7D, 0x05,                                 // jge   .continue >---*
    REX_WRB, 0x31, 0xDB,                        // xor   r11, r11      |
    0xF7, 0xDE,                                 // neg   rsi           |
                                                //                     |
    PUSH_REG + RCX,                             // push  rcx <---------*
    MOV_NUM + RBX, 0x0A,0x00,0x00,0x00,         // mov   ebx, 10          
    REX_W, 0x8D, 0x7C, 0x24, 0x10,              // lea   rdi, [rsp + 16]
    STD,                                        // std
                                                //
    XOR,  OP(3, RDX, RDX),                      // xor   edx, edx <--*
    MOV_REG, OP(3, RSI, RAX),                   // mov   eax, esi    |
    0xF7, 0xF3,                                 // div   ebx         |
    MOV_REG, OP(3, RAX, RSI),                   // mov   esi, eax    |
    0x88, 0xD0,                                 // mov   al, dl      |
    0x04, 0x30,                                 // add   al, '0'     |
    STOSB,                                      // stosb             |
    REX_W, 0xFF, DEC + RCX,                     // dec   rcx         |
    0x75, 0xEE,                                 // jne   .convert >--*
                                                //
    POP_REG + RCX,                              // pop   rcx
    REX_W, 0xFF, INC + RCX,                     // inc   rcx
    0xB0 + RAX, 0x2E,                           // mov   al, '.'
    STOSB,                                      // stosb
                                                //
    XOR,  OP(3, RDX, RDX),                      // xor   edx, edx <--*
    MOV_REG, OP(3, RSI, RAX),                   // mov   eax, esi    |
    0xF7, 0xF3,                                 // div   ebx         |
    MOV_REG, OP(3, RAX, RSI),                   // mov   esi, eax    |
    0x88, 0xD0,                                 // mov   al, dl      |
    0x04, 0x30,                                 // add   al, '0'     |
    STOSB,                                      // stosb             |
    REX_W, 0xFF, INC + RCX,                     // inc   rcx         |
    0x83, 0xFE, 0x00,                           // cmp   esi, 0      |
    0x75, 0xEB,                                 // jne   .convert >--*
                                                //
    REX_WB, 0x83, 0xFB, 0x00,                   // cmp   r11, 0 
    0x75, 0x08,                                 // jne   .positive >-----*
    0xC6, 0x07, 0x2D,                           // mov   byte [rdi], '-' |
    REX_W, 0xFF, INC + RCX,                     // inc   rcx             |
    0xEB, 0x03,                                 // jmp   .print >--------+--*
    REX_W, 0xFF, INC + RDI,                     // inc   rdi <-----------*  |
                                                //                          |
    MOV_NUM + RAX, 0x01,0x00,0x00,0x00,         // mov   eax, 1   ;write <--*
    REX_W, MOV_REG, OP(3, RDI, RSI),            // mov   rsi, rdi ;from [rdi]
    MOV_NUM + RDI, 0x01,0x00,0x00,0x00,         // mov   edi, 1   ;to STDOUT
    REX_W, MOV_REG, OP(3, RCX, RDX),            // mov   rdx, rcx ;RCX symbols
    WIDE_OP, SYSCALL,                           // syscall
                                                //
    REX_W, 0x83, 0xC4, 0x10,                    // add   rsp, 16
    REX_B, POP_REG + R11,                       // pop   r11
    RET_NEAR                                    // ret
};

#endif
