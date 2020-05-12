#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#pragma pack(1)
struct Argument {
	char int_param : 1;     // val field is valid
	char reg_param : 1;     // reg field is valid
	char mem_param : 1;     
	char sum_param : 1;      
	char nop	   : 4;
	char reg;               // register id
	int  val;               // int value

    Argument() = default;

    Argument(const Argument& other) = default;

    Argument(Argument&& other) = default;

    ~Argument() = default;
};
#pragma pack()

#endif //ARGUMENT_HPP
