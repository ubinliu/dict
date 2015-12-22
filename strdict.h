/*
 
 * ============================================================
 
 *       Filename:  strdict.h
 *    Description:  
 *         Author:  liuyoubin@ubinliu.com
 *        Company:  ubinliu.com, Inc
 *        Created:  2015-12-17 16:43:33
 
 * ============================================================
 
 */

#ifndef __STR_DICT_
#define __STR_DICT_
#include <stdio.h>
#include <stdlib.h>
#include "dict.h"

unsigned int _str_hash(const void *key, int len);
void* _str_dup(const void *src);
void _str_free(void *str);
int _str_cmp(const void *key1, const void *key2);
operation_t * str_operation();

#endif
