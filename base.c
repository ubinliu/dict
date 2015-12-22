/*
 
 * ============================================================
 
 *       Filename:  base.cpp
 *    Description:  
 *         Author:  liuyoubin@ubinliu.com
 *        Company:  ubinliu.com, Inc
 *        Created:  2015-05-25 10:32:35
 
 * ============================================================
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "dict.h"
#include "strdict.h"

unsigned long get_ms() {
        struct timeval now;
    gettimeofday(&now,NULL);
    return (unsigned long)now.tv_sec * 1000 + (unsigned long)now.tv_usec/1000;
}

int main(int argc, char *argv[]){

    (void)argc;
    (void)argv;
    operation_t *operation = str_operation();

    dict_t *dict = dict_create(operation);
    int i;
    char num[65];
    unsigned long begin = get_ms();
    for (i = 0; i < atoi(argv[1]); ++i){
        snprintf(num, 65, "%d", i);
        dict_set(dict, num, num);
    }
    unsigned long set_end = get_ms();
        
    char * a = (char *)dict_get(dict, "2");
    printf("a:%s\n", a);
    char * b = (char *)dict_get(dict, "22");
    printf("b:%s\n", b);
    char * c = (char *)dict_get(dict, "222");
    printf("c:%s\n", c);
    char * d = (char *)dict_get(dict, "2222");
    printf("d:%s\n", d);
    
    unsigned long get_end = get_ms();
    printf("rehash:%d, ht0 size:%d, used:%d, ht1 size:%d, used:%d, set_cost:%lu, get_cost:%lu\n",
        dict->rehash,
        dict->hash_table[0].size, dict->hash_table[0].used,
        dict->hash_table[1].size, dict->hash_table[1].used,
        set_end-begin, get_end - set_end);

    for (i = 0; i < (atoi(argv[1]) - atoi(argv[1]) / 100); ++i){
        snprintf(num, 65, "%d", i);
        dict_del(dict, num);
    }
    unsigned long del_end = get_ms();

    printf("rehash:%d, ht0 size:%d, used:%d, "
        "ht1 size:%d, used:%d, set_cost:%lu, get_cost:%lu, del_cost:%lu\n",
        dict->rehash,
        dict->hash_table[0].size, dict->hash_table[0].used,
        dict->hash_table[1].size, dict->hash_table[1].used,
        set_end-begin, get_end - set_end, del_end-get_end);
    //dict_dump(dict);
    char * e = (char *)dict_get(dict, "2222");
    printf("e:%s\n", e);
    sleep(1000);
    return 0;
}
