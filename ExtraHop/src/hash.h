/*
 ============================================================================
 Name        : hash.h
 Author      : Abhishek Maurya
 Version     :
 Copyright   : None
 Description : Programming Challenge for ExtraHop
 ============================================================================
 */

#ifndef HASH_H_
#define HASH_H_
#include <stdint.h>
#include <stdlib.h>
//#define DEBUG_ENABLE

/*GCC optimization needs to be turned on */
#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

/* Not a secure way */
#define PRINTF_OUT(fp,msg,args...) do {   							\
		fprintf(fp,"\n"msg, ##args);\
		fflush(stdout);\
	}while(0);

#define DEBUG_ERR(msg, args...) do {   							\
		printf("\nERR [%s:%d] "msg, __func__, __LINE__, ##args);\
		fflush(stdout);\
	}while(0);


#ifdef DEBUG_ENABLE
#define DEBUG_INFO(msg, args...) do {	\
		printf("\n"msg, ##args);		\
		fflush(stdout);\
	}while(0);
#else
#define DEBUG_INFO(msg, args...)
#endif

typedef struct hashentry{
	char *key;
	uint64_t value;
	struct hashentry* next;
}hashentry_t;


typedef struct hashtable {
	hashentry_t **entrytable;
	/* Number of hash collision
     * Used to compute better hash algo
	 */
	size_t  size;
	uint64_t collision_count;
} hashtable_t;

extern hashtable_t *init_hash_table(size_t size);
extern int32_t hash_put(hashtable_t* hashtable, const char *key, uint64_t value);
extern int32_t hash_value_incr(hashtable_t* hashtable, const char *key, uint64_t incr);
extern void  hash_dump(hashtable_t* hashtable, FILE *outfile);
extern void  destroy_hash_table(hashtable_t* hashtable);
#endif /* HASH_H_ */

