#ifndef SAS_INSTRUCTIONS_HPP
#define SAS_INSTRUCTIONS_HPP

#include <cstdlib>

#include "../simple_vector/vector.hpp"
#include "argument.hpp"

struct Instruction {

    static void set_buf_begin(const char* begin);
    static void resize_offsets(const size_t& size);
    
    static char* write_preamble(char* dest);

    //

    Instruction() = delete;

    Instruction(const char* op_ptr);

    Instruction(const Instruction& other) = delete;

    Instruction& operator=(const Instruction& other) = delete;

    Instruction(Instruction&& other);

    Instruction& operator=(Instruction&& other) = delete;

    ~Instruction() = default;

    size_t n_args() const;

    void set_addr(char* addr) const;

    char* write() const;

    void fix_jmp() const;

    Vector<const Argument*> arg;
    const char* opcode;

private:

    static const char* buf_begin;
    static Vector<char*> offsets;

    static const char* IN_PTR;
    static const char* OUT_PTR;
    static const char* OUTC_PTR;

    // TO BE GENERATED
    char* write_END   (char* dest) const;
    char* write_MATH  (char* dest) const;
    char* write_MUL   (char* dest) const;
    char* write_DIV   (char* dest) const;
    char* write_PUSH  (char* dest) const;
    char* write_POP   (char* dest) const;
    char* write_IN    (char* dest) const;
    char* write_OUT   (char* dest) const;
    char* write_OUTC  (char* dest) const;
    char* write_MOV   (char* dest) const;
    char* write_JMP   (char* dest) const;
    char* write_JCOND (char* dest) const;
    char* write_RET   (char* dest) const;
    char* write_SQRT  (char* dest) const;
};

#endif//SAS_INSTRUCTIONS_HPP    
