/*
 
 * ============================================================
 
 *       Filename:  dict.c
 *    Description:  
 *         Author:  liuyoubin@ubinliu.com
 *        Company:  ubinliu.com, Inc
 *        Created:  2015-12-15 23:52:07
 
 * ============================================================
 
 */

#include "dict.h"
#include "apue.h"



dict_t *dict_create(operation_t *operation){
    dict_t *d = (dict_t*)malloc(sizeof(dict_t));
    if (d == NULL){
        perror("oom");
        exit(-1);
    }

    d->operation = operation;
    d->rehash = 0;
    d->hash_table[0].size = DEFAULT_BUCKET_SIZE;
    d->hash_table[0].used = 0;
    d->hash_table[0].kvs = (kv_pair_t**)malloc(DEFAULT_BUCKET_SIZE*sizeof(kv_pair_t*));
    int i;
    for (i = 0; i < DEFAULT_BUCKET_SIZE; ++i){
        d->hash_table[0].kvs[i] = NULL;
    }
    return d;
}

void dict_destroy(dict_t *d){
    
    unsigned int i = 0;
    unsigned int j = 0;
    kv_pair_t *kv = NULL;
    kv_pair_t *next = NULL;
    for (j = 0; j < 2; ++j){
        for (i = 0; i < d->hash_table[j].size; ++i){
            kv = d->hash_table[j].kvs[i];
            while (kv != NULL){
                next = kv->next;
                d->operation->free_key(kv->key);
                d->operation->free_value(kv->value);
                kv = next;
            }
        }
        free(d->hash_table[j].kvs);
        d->hash_table[j].kvs = NULL;
    }

    free(d);
    d = NULL;
}

void dict_dump(dict_t *d){
    unsigned int i = 0;
    unsigned int j = 0;
    kv_pair_t *kv = NULL;
    kv_pair_t *next = NULL;

    printf("dict dump\n");
    printf("rehash:\t%d\n", d->rehash);

    for (j = 0; j < 2; ++j){
        printf("hash_table:\t%d\tsize:\t%d\tused:\t%d\n",
            j, d->hash_table[j].size, d->hash_table[j].used);
        for (i = 0; i < d->hash_table[j].size; ++i){
            
            int kvs = 0;
            kv = d->hash_table[j].kvs[i];
            while (kv != NULL){
                ++kvs;
                printf("key:\t%s,value:\t%s\n", (char *)kv->key, (char *)kv->value);
                next = kv->next;
                kv = next;
            }
            printf("idx:%d,total:%d\n", i, kvs);
        }        
    }
}

void* dict_get(dict_t *d, const void *key){
    
    need_rehash(d);

    hash_table_t *ht = &(d->hash_table[0]);
    long idx = -1;
    if (d->rehash){
        idx = dict_index_of(d, key, ht->size);
        
        if (ht->kvs[idx] != NULL){
            _rehash_one(d, idx);
        }

        idx = -1;
        ht = &(d->hash_table[d->rehash]);
        
    }

    if (idx == -1){
        idx = dict_index_of(d, key, ht->size);
    }

    kv_pair_t* kv = ht->kvs[idx];
    while(kv != NULL){
        if (d->operation->cmp_key(key, kv->key) == 1){
            return kv->value;
        }
        kv = kv->next;
    }    

    return NULL;
}

void dict_set(dict_t *d, const void* key, const void * value){

    need_rehash(d);

    hash_table_t *ht = &(d->hash_table[0]);
    long idx = -1;

    if (d->rehash){
        idx = dict_index_of(d, key, ht->size);
        if (ht->kvs[idx] != NULL){
            _rehash_one(d, idx);
        }

        idx = -1;
        ht = &(d->hash_table[d->rehash]);
    }

    if (idx == -1){
        idx = dict_index_of(d, key, ht->size);
    }
    
    kv_pair_t *kv = ht->kvs[idx];
    while(kv != NULL){
        if (1 == d->operation->cmp_key(kv->key, key)){
            d->operation->free_value(kv->value);
            kv->value = d->operation->dup_value(value);
            return;
        }
        kv = kv->next;
    }

    kv = (kv_pair_t*)malloc(sizeof(kv_pair_t));
    if (kv == NULL){
        perror("oom");
        exit(-1);
    }

    ++(ht->used);
    
    kv->key = d->operation->dup_key(key);
    kv->value = d->operation->dup_value(value);

    kv->next = ht->kvs[idx];
    ht->kvs[idx] = kv;

    return;
}

void dict_del(dict_t *d, const void *key){
    
    need_rehash(d);

    hash_table_t *ht = &(d->hash_table[0]);
    long idx = -1;
    if (d->rehash){
        idx = dict_index_of(d, key, ht->size);
        if (ht->kvs[idx] != NULL){
            _rehash_one(d, idx);
        }        
        
        idx = -1;

        ht = &(d->hash_table[d->rehash]);
    }

    if (idx == -1){
        idx = dict_index_of(d, key, ht->size);
    }
    
    kv_pair_t *kv = ht->kvs[idx];
    kv_pair_t *head = kv;

    while(kv != NULL){
        if (d->operation->cmp_key(key, kv->key) == 1){
            break;
        }
        head = kv;
        kv = kv->next;
    }

    if (kv == NULL){
        return;
    }  

    ht->used -= 1;

    if (ht->kvs[idx] == kv){
        ht->kvs[idx] = kv->next;
    }else{
        head->next = kv->next;
    }    
    
    d->operation->free_key(kv->key);
    d->operation->free_value(kv->value);
    kv->next = NULL;
    free(kv);
}

long dict_index_of(dict_t *d, const void *key, unsigned int bucket_size){
    long idx = d->operation->hash(key, strlen(key));
    return idx & (bucket_size - 1);
}

void _rehash_one(dict_t *d, long idx){
    hash_table_t* ht = &(d->hash_table[0]);    

    //rehash a bucket
    kv_pair_t *kv = ht->kvs[idx];
    kv_pair_t *next = NULL;
    long idx2 = -1;
    kv_pair_t *head2 = NULL;

    while(kv != NULL){
        next = kv->next;
        idx2 = dict_index_of(d, kv->key, d->hash_table[1].size);
        head2 = d->hash_table[1].kvs[idx2];
        d->hash_table[1].kvs[idx2] = kv;
        kv->next = head2;

        kv = next;

        d->hash_table[0].used -= 1;
        d->hash_table[1].used += 1;
    }

    ht->kvs[idx] = NULL;

    //check if rehash finished
    int rehash_finished = 1;
    unsigned int i = 0;
    for (i = 0; i < ht->size; ++i){
        if (ht->kvs[i] != NULL){
            rehash_finished = 0;
            break;
        }
    }
    
    if (rehash_finished){
        d->rehash = 0;
        free(d->hash_table[0].kvs);
        d->hash_table[0].size = d->hash_table[1].size;
        d->hash_table[1].size = 0;
        d->hash_table[0].used = d->hash_table[1].used;
        d->hash_table[1].used = 0;
        d->hash_table[0].kvs = d->hash_table[1].kvs;
        d->hash_table[1].kvs = NULL;
    }
}

void dict_rehash(dict_t *d, int steps){
    int i = 0;
    for (i = 0; i < steps; ++i){
        if (d->hash_table[0].kvs[i] != NULL){
            _rehash_one(d, i);
        }
    }
}

void need_rehash(dict_t *d){
    if (d->rehash == 1){
        return;
    }

    float fill = ((float)d->hash_table[0].used) / (float)d->hash_table[0].size;
    long bucket_size = DEFAULT_BUCKET_SIZE;

    if (fill > 5){
        d->rehash = 1;
        long need_resize = d->hash_table[0].used * 2;

        while (bucket_size < need_resize){
            bucket_size = bucket_size * 2;
        }
    }else if (fill < 0.1 && fill > 0){
        d->rehash = 1;
        while (bucket_size < d->hash_table[0].used){
            bucket_size = bucket_size * 2;
        }
    }else{
        return;
    }

    d->hash_table[1].size = bucket_size;
    d->hash_table[1].used = 0;
    d->hash_table[1].kvs = (kv_pair_t **)malloc(bucket_size * sizeof(kv_pair_t*));
  
    int i = 0;
    for (i = 0; i < bucket_size; ++i){
        d->hash_table[1].kvs[i] = NULL;
    }
}
