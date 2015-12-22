/*
 
 * ============================================================
 
 *       Filename:  strdict.c
 *    Description:  
 *         Author:  liuyoubin@ubinliu.com
 *        Company:  ubinliu.com, Inc
 *        Created:  2015-12-17 16:43:40
 
 * ============================================================
 
 */

#include <string.h>
#include "strdict.h"

unsigned int _str_hash(const void *key, int len){
    /* 'm' and 'r' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.  */
    unsigned int seed = 5381;
    unsigned int m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */
    unsigned int h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */
    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        unsigned int k = *(unsigned int*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated. */
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

operation_t * str_operation(){
    operation_t *op = (operation_t*)malloc(sizeof(operation_t));
    if (op == NULL){
        perror("oom");
        exit(-1);
    }
    op->hash = _str_hash;
    op->dup_key = _str_dup;
    op->dup_value = _str_dup;
    op->free_key = _str_free;
    op->free_value = _str_free;
    op->cmp_key = _str_cmp;
    return op;
}


void* _str_dup(const void *src){
    const char* src_str = (const char *) src;
    if (src_str == NULL){
        return NULL;
    }

    unsigned int len = strlen(src_str);
    
    char* dest_str = (char *) malloc(sizeof(char) * (len + 1));
    if (dest_str == NULL){
        perror("oom");
        exit(-1);
    }

    snprintf(dest_str, len + 1, "%s", src_str);

    return dest_str;
}

void _str_free(void *key){
    free(key);
    key = NULL;
}

int _str_cmp(const void *key1, const void *key2){
    const char * key_1 = (const char *)key1;
    const char * key_2 = (const char *)key2;
    int result = strcmp(key_1, key_2);
    return result == 0 ? 1 : 0;
}

