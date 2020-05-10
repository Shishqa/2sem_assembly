#ifndef SAS_INSTRUCTIONS_HPP
#define SAS_INSTRUCTIONS_HPP

#include <cstdlib>

class Instruction {
public:
    Instruction() = delete;
    Instruction(const Instruction& other) = delete;
    Instruction(Instruction&& other) = delete;

    Instruction(const char* op_ptr);

    ~Instruction();

    size_t size() const;

    size_t n_args() const;

    size_t write(char* dest) const;

private:
    char* buffer;
    unsigned int  offset;
    unsigned char buf_size;
    unsigned char arg_cnt;
    char opcode;
    bool is_active;

    // TO BE GENERATED
    
    void encode_END();
    void encode_PUSH(const char* arg_ptr);
    void encode_POP(const char* arg_ptr);
    void encode_ARITHMETICS();
    //void encode_POP(const char* op_ptr);
    //void encode_PUSH(const char* op_ptr);
    //void encode_OUT(const char* op_ptr);
    //void encode_ADD(const char* op_ptr);
};

#endif//SAS_INSTRUCTIONS_HPP    
