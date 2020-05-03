/*
 * config.h
 *
 *  Created on: 21 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#pragma pack(1)
struct Signature
{
    const char sign[5]    = "SHSH";
    const double version  = 12.0;
    const short int day   = 25;
    const short int month = 10;
    const short int year  = 2019;
    const char label[6]   = "START";
};
#pragma pack()

const char COMMENT_MARKER = ';';

const int USER_REG_NUM = 16;
const int REG_LEN = 3;

const char reg_names[USER_REG_NUM][REG_LEN] = { "ax", "bx", "cx", "dx",
												"ex", "fx", "gx", "hx",
			      	  	  	  	            	"al", "bl", "cl", "dl",
								                "ah", "bh", "ch", "dh" };

const int HIDDEN_REG_NUM = 4;

const int RAM_SIZE = 100000;

const int VRAM_SIZE = 100000;

const int LABEL_MAX_CNT = 1000;

const int LABEL_MAX_LEN = 1000;

const int MAX_ARG_LEN = 100;

const int MAX_CMD_LEN = 100;

const int MAX_OUTPUT = 5000;

const int MAX_RUN = 2;

const int MAX_CALL = 1000;


#endif /* CONFIG_H_ */
