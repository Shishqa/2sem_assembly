/*
 * ms_label.h
 *
 *  Created on: 27 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_LABEL_H_
#define MS_LABEL_H_

#include <stdio.h>

enum
{
	PARSED = 15,
	NOT_PARSED = 16,
	FAILURE = 17
};

class Label
{
private:
	int call_max;

public:
	char **calls;
	int call_num;
	char *name;
	size_t name_len;
	int ptr;

	void init(char *name, size_t len, int ptr, int call_max);
	void clear();

	bool addCall(char *from);
};

class Labels
{
private:
	int cnt;
	Label *data;
	int max_cnt;
	int call_one_max;

public:
	void init(int size, int call_one_max);
	void clear(void);

	bool add(char *name, size_t len, int ptr);
	int call(char *name, size_t len, char *out_curr);
	int getPtr(char *name, size_t len);
	char *getName(int ptr);
	int getNum(int ptr);
	bool fillNames();
	bool fillPointers();
	void printUndef();
};

#endif /* MS_LABEL_H_ */
