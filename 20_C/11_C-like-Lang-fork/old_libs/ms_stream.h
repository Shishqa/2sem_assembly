/*
 * ms_stream.h
 *
 *  Created on: 12 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_STREAM_H_
#define MS_STREAM_H_

#include <stdio.h>

size_t GetSizeOfFile (FILE* stream);

size_t Read (char** buffer, FILE* stream);

size_t GetIntLen (int x);

int SkipSpaces (char** buffer);


#endif /* MS_STREAM_H_ */
