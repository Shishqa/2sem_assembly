#ifndef SAS_INSTRUCTIONS_HPP
#define SAS_INSTRUCTIONS_HPP

#include <cstdlib>

#include "../simple_vector/vector.hpp"
#include "argument.hpp"

struct Instruction {

    Instruction();

    Instruction(const char* op_ptr);

    Instruction(const Instruction& other) = delete;

    Instruction(Instruction&& other);

    ~Instruction() = default;

    size_t n_args() const;

    size_t write(char* dest) const;

    Vector<const Argument*> arg;
    size_t jmp_target_idx;
    const char* opcode;

private:
    // TO BE GENERATED
    
    size_t write_END (char* dest) const;
    size_t write_MATH(char* dest) const;
    size_t write_PUSH(char* dest) const;
    size_t write_POP (char* dest) const;


    void encode_PUSH(const char* arg_ptr);
    void encode_POP(const char* arg_ptr);
    void encode_ARITHMETICS();
    //void encode_POP(const char* op_ptr);
    //void encode_PUSH(const char* op_ptr);
    //void encode_OUT(const char* op_ptr);
    //void encode_ADD(const char* op_ptr);
};

#endif//SAS_INSTRUCTIONS_HPP    
