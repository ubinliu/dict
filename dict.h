/*
 
 * ============================================================
 
 *       Filename:  dict.h
 *    Description:  
 *         Author:  liuyoubin@ubinliu.com
 *        Company:  ubinliu.com, Inc
 *        Created:  2015-12-15 23:51:07
 
 * ============================================================
 
 */

#ifndef __DICT_
#define __DICT_
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_BUCKET_SIZE 4

typedef struct kv_pair_s kv_pair_t;

struct kv_pair_s{
    void *key;
    void *value;
    kv_pair_t *next;
};

typedef struct hash_table_s{
    kv_pair_t **kvs;
    unsigned int size;
    unsigned int used;
}hash_table_t;

typedef struct operation_s{
    unsigned int (*hash)(const void *key, int len);
    void* (*dup_key)(const void *src_key);
    void* (*dup_value)(const void *src_value);
    void (*free_key)(void *key);
    void (*free_value)(void *value);
    int (*cmp_key)(const void *key1, const void *key2);
}operation_t;

typedef struct dict_s{
    operation_t *operation;
    hash_table_t hash_table[2];
    int rehash;
}dict_t;

dict_t *dict_create(operation_t *operation);
void dict_destroy(dict_t *d);
void dict_dump(dict_t *d);
void* dict_get(dict_t *d, const void *key);
void  dict_set(dict_t *d, const void* key, const void * value);
void  dict_del(dict_t *d, const void *key);
long dict_index_of(dict_t *d, const void *key, unsigned int bucket_size);
void _rehash_one(dict_t *d, long idx);
void dict_rehash(dict_t *d, int steps);
void reset_hash_table(hash_table_t *ht);
void need_rehash(dict_t *d);
#endif
