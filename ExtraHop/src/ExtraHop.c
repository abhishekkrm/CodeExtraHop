/*
 ============================================================================
 Name        : ExtraHop.c
 Author      : Abhishek Maurya
 Version     :
 Copyright   : Feel free to use this code
 Description : Programming Challenge for ExtraHop
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExtraHop.h"
#include "hash.h"
/*
 * Function to parse tracking http header provided in the file and store
 * them in HASH TABLE for O(1) access later on.
 */

void parse_track_headers(const char *filename, hashtable_t* hashtable) {
	/* File Pointer*/
	FILE *fp = NULL;
	char *line = NULL; /*Read line */
	size_t len = 0;
	ssize_t read = 0;
	char *token = NULL;

	fp = fopen(filename, "r");
	if (unlikely(fp == NULL)) {
		DEBUG_ERR("Unable to open file: %s", filename);
		exit(EXIT_FAILURE);
	}
	/* if line is passed as null getline will allocate buffer
	 * However application has to free later.
	 */
	while ((read = getline(&line, &len, fp)) != -1) {
		// Process line
		token = strtok(line,"\n");
		token = strtok(token," ");
		/* Comment Skip */
		if (unlikely(NULL == token)) {
			continue;
		}
		//DEBUG_INFO("Track Token %s",token);

		/* Put key value pair in hash table */
		/* Initialize value with Zero */
		/* Returns Zero on success else -1*/
		if (-1 == hash_put(hashtable,token,0)){
			DEBUG_ERR("Memory Allocation failed for Hash Entry");
		}
	}
	// free line buffer
	free(line);
	// close file pointer
	fclose(fp);
}
/*
 * Function to process the headers file and increment the count if the
 * header already exits in HashTable
 */
void process_header(const char *filename,hashtable_t* hashtable) {
	FILE *fp = NULL;
	char *line = NULL;
	size_t len = 0;
	ssize_t read = 0;
	char *token = NULL;

	fp = fopen(filename, "r");
	if (unlikely(fp == NULL)) {
		DEBUG_ERR("Unable to open file: %s", filename);
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		token = strtok(line, HTTP_RFC_DELIMITER);

		if (unlikely(NULL == token)) {
			continue;
		}
		//DEBUG_INFO("Parse Token %s",token);

		/* Returns Zero on success else -1*/
		/* Increment the value with 1 if token present */
		if(0 == (hash_value_incr(hashtable, token, 1))) {
			DEBUG_INFO("%s is present in HashTable", token);
		}
	}
	free(line);
	fclose(fp);
}
/*
 * Program execution begin heres
 */
int main(void) {

	hashtable_t* hashtable = NULL;

	hashtable = init_hash_table(HASH_SIZE);

	if (unlikely(hashtable == NULL)) {
		DEBUG_ERR("Unable to Create Hash Table");
		exit(EXIT_FAILURE);
	}
	/* Parse Tracking header and store in hash */
	parse_track_headers(TRACK_HEADERS_FILE, hashtable);

	/*Process Headers and increment value if present in HashTable */
	process_header(HEADERS_FILE, hashtable);

	FILE * fout = fopen(OUTPUT_FILE, "w");
	if (unlikely(fout == NULL)) {
		DEBUG_ERR("Unable to open file: %s", OUTPUT_FILE);
		exit(EXIT_FAILURE);
	}

	/* Print the out on fout stream */
	hash_dump(hashtable, fout);

	/*
	 *  Free up the memory allocated in hashtable
	 */
	destroy_hash_table(hashtable);

	fclose(fout);
	return EXIT_SUCCESS;
}
