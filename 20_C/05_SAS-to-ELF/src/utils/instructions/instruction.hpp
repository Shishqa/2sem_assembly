#ifndef SAS_INSTRUCTIONS_HPP
#define SAS_INSTRUCTIONS_HPP

#include <cstdlib>

#include "../simple_vector/vector.hpp"
#include "argument.hpp"

struct Instruction {

    static void set_buf_begin(const char* begin);
    static void resize_offsets(const size_t& size);

    static const char* get_buf_begin();

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

    char* write(char* dest) const;

    void fix_jmp() const;

    void mute();

    Vector<const Argument*> arg;
    const char* opcode;

private:

    bool is_active;

    static const char* buf_begin;
    static Vector<char*> offsets;

    static const char* IN_PTR;
    static const char* OUT_PTR;
    static const char* OUTC_PTR;
    static const char* OUTF_PTR;

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
    char* write_OUTF  (char* dest) const;
};

#endif//SAS_INSTRUCTIONS_HPP    
