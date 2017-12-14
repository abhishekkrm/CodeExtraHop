/*
 ============================================================================
 Name        : hash.c
 Author      : Abhishek Maurya
 Version     :
 Copyright   : None
 Description : Programming Challenge for ExtraHop
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/*
 * A simple Hash compute function on string
 */
static inline uint32_t hash_compute(const char * str, size_t  size) {
	  int len =strlen(str);
	  int sum=0,i;
	  for (i=0; i < len; i++)
	    sum += str[i];
	  return (sum % size);
}

/*
 * Hash Map init function, Initializes Hash of given Size
 */
hashtable_t* init_hash_table(size_t size) {

	//int i;
	hashtable_t *hashtable = NULL;
	 hashtable = (hashtable_t *)calloc(1, sizeof(hashtable_t));
	 if (likely(NULL != hashtable)) {
		 /* Entry Table initialized with NULL Calloc*/
		 hashtable->entrytable = (hashentry_t **)calloc(1, sizeof(hashentry_t*)*size);
		 if (unlikely(NULL == hashtable->entrytable)) {
			 DEBUG_ERR("Memory Allocation Failed while initializing HashEntry");
			 free(hashtable);
			 return (NULL);
		 }
		 hashtable->size = size;
		 hashtable->collision_count = 0;
	 }

	 // calloc
	 //for (i=0;i<size;i++)
	 //	 hashtable->entrytable[i] = NULL;
	 return (hashtable);
}

/*
 * Create memory for new Hash Entry (key value pair)
 */
// Can be non-Inline
static inline hashentry_t* new_entry(const char *key, uint64_t value) {

	hashentry_t* hashentry = (hashentry_t *)malloc(sizeof(hashentry_t));
	 if (likely((NULL != hashentry))) {
		 uint64_t key_len = strlen(key);
		 hashentry->key = (char*)malloc(sizeof(char)*(key_len+1));
		 if(unlikely(NULL == hashentry->key)) {
			 DEBUG_ERR("Memory Allocation Failed for Hash Key");
			 free(hashentry);
			 return NULL;
		 }

		 /* Copy safe */
		 strncpy(hashentry->key,key,key_len);
		 hashentry->key[key_len] ='\0';
		 hashentry->value = value;
		 hashentry->next = NULL;
		 DEBUG_INFO("HashMapInserted Key:%s Value %lu",hashentry->key, hashentry->value);
	}
	return hashentry;
}

/*
 *  Hash function API to put key-value pair in hash
 */
int32_t hash_put(hashtable_t* hashtable,const char *key, uint64_t value) {
	hashentry_t *hashentry = NULL;
	uint64_t hash_value = 0;
	hash_value = hash_compute(key, hashtable->size);
	/*Get Hash Entry*/
	hashentry= hashtable->entrytable[hash_value];
	/*
	 * Likely because collision in hash table are not frequent
	 * However, if collision are more change hash function
	 */
	if (likely(NULL == hashentry)) {
		/* Insert new key value pair */
		hashentry_t * hashentry_new =  new_entry(key,value);

		if(likely(hashentry_new)){
			hashtable->entrytable[hash_value] = hashentry_new;
			hashentry = hashentry_new;
		}//else Memory Allocation failure return NULL;
	} else {
			/* Hash Collision */
			while (hashentry) {
				if(0 == strcmp(hashentry->key, key))
					break;
				hashentry=hashentry->next;
			}
			/* Key Not Found, Insert new key value pair*/
			if (NULL == hashentry){
				hashentry_t * hashentry_new =  new_entry(key,value);
				if(hashentry_new) {
					hashtable->collision_count++;
					DEBUG_INFO("HashMap Collision %lu",hashtable->collision_count);
					hashentry_new->next = hashtable->entrytable[hash_value];
					hashtable->entrytable[hash_value] = hashentry_new;
					hashentry = hashentry_new;
				}
			}else {
				// Update the Value for the key
				DEBUG_INFO("Update HashMap %s:%lu",hashentry->key, value);
				hashentry->value = value;
		  }
	}
	/*
	 * Lets not return HashEntry pointer to the caller,
	 * as it can maliciously exploit the API
	 */
	return (hashentry ? (0):(-1));
}

/*
 * Lets not implement get as again we need to call put
 * Assuming we are not returning entry pointer.
 */
/*
 * Function to increment value corresponding to Key if the  value not present
 * return -1
 */
int32_t hash_value_incr(hashtable_t* hashtable,const char *key,uint64_t incr) {
	hashentry_t *hashentry = NULL;
	uint64_t hash_value = 0;
	hash_value = hash_compute(key, hashtable->size);
	/*Get Hash Entry*/
	hashentry= hashtable->entrytable[hash_value];

	if (likely(NULL != hashentry)) {
		while (hashentry) {
			if(0 == strcmp(hashentry->key,key))
				break;
			hashentry=hashentry->next;
		}
		/* Update Hash Value */
		if(hashentry) {
			hashentry->value += incr;
		}

	}
	/*
	 * Lets not return HashEntry pointer to the caller,
	 * as it can maliciously exploit the API
	 */
	return (hashentry ? (0):(-1));
}
/*
 * Function to dump Hash Table
 */
void hash_dump(hashtable_t* hashtable, FILE *output) {
	/* Let's store this locally rather than comparing in loop
 	 * Not really required as compiler may save value in register 
 	 */
	int i;
 	size_t size =  hashtable->size;
	PRINTF_OUT(output,"------------- HTTP Header Occurrance  -------");
	PRINTF_OUT(output,"Header : Count");
	PRINTF_OUT(output,"----------------------------------------------");
	hashentry_t *hashentry = NULL;
	for (i = 0 ; i< size; i++) { 
		if (hashtable->entrytable[i]) {
			hashentry = hashtable->entrytable[i];
			while (hashentry) {
				PRINTF_OUT(output,"%s : %lu", hashentry->key, hashentry->value);
				hashentry = hashentry->next;
			}
		}
	 }
	PRINTF_OUT(output,"----------------------------------------------\n");
}
/*
 * Function to destroy Hash Table
 */
void  destroy_hash_table(hashtable_t* hashtable) {
	int i;
 	size_t size =  hashtable->size;
	hashentry_t * hashentry = NULL,*temp;
	for (i = 0 ; i< size; i++) {
		if (hashtable->entrytable[i]) {
			hashentry = hashtable->entrytable[i];
			while (hashentry) {
				temp = hashentry;
				hashentry = hashentry->next;
				free(temp->key);
				free(temp);
			}
		}
	}
	free(hashtable->entrytable);
	free(hashtable);
}






