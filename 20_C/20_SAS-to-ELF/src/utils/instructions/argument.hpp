#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#pragma pack(1)
struct Argument {
	char int_param : 1;     // val field is valid
	char reg_param : 1;     // reg field is valid
	char mem_param : 1;     
	char sum_param : 1;     // wasn't implemented ? 
	char nop	   : 4;
	char reg;               // register id
	int  val;               // int value

    Argument() = delete;
    Argument(const Argument& other) = delete;
    Argument(Argument&& other) = delete;

    explicit Argument(const char* arg_ptr);

    ~Argument() = default;
};
#pragma pack()

#endif //ARGUMENT_HPP
