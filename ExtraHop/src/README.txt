Design:
In order to achieve O(FileSize) complexity [Assumption FilzeSize is much bigger
than Http tracking Headers] a HashTable is implemented. Initially the Hash Table
is build from the tracking header and initialized with count value 0. Whenever the
header match with the HashTable key increment corresponding value.Default HashSize
is 4096 can be changed in ExtraHop.h file [#define HASH_SIZE 4096]   


Input Format:
Tracking Header : Need to be provided in "trackHeader.txt"
Header file:      Need to be provided in "header.txt"

OutPut Format:
Default: output is directed to /dev/stdout

Can be change by modifying ExtraHop.h header file:
#define TRACK_HEADERS_FILE "trackHeader.txt"
#define HEADERS_FILE "header.txt"
//#define OUTPUT_FILE "output.txt"
#define OUTPUT_FILE "/dev/stdout"


Requirement:
Linux GCC compiler

Steps to RUN: 
1. cd to src folder 
2. make
3. ./ExtraHop

Test case for the sample Input:
abhi@OptimusPrime:~/workspace/ExtraHop/src$ make
gcc -c -g -Wall -o ExtraHop.o ExtraHop.c
gcc -c -g -Wall -o hash.o hash.c
gcc -ggdb -o  ExtraHop ExtraHop.o hash.o
abhi@OptimusPrime:~/workspace/ExtraHop/src$ valgrind --leak-check=full ./ExtraHop 
==7644== Memcheck, a memory error detector
==7644== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==7644== Using Valgrind-3.10.0.SVN and LibVEX; rerun with -h for copyright info
==7644== Command: ./ExtraHop
==7644== 

------------- HTTP Header Occurrence  -------
Header : Count
----------------------------------------------
Test : 0
Accept : 21
User-Agent : 35
Connection : 7
Content-Length : 28
Content-Encoding : 7
----------------------------------------------
==7644== 
==7644== HEAP SUMMARY:
==7644==     in use at exit: 0 bytes in 0 blocks
==7644==   total heap usage: 19 allocs, 19 frees, 34,946 bytes allocated
==7644== 
==7644== All heap blocks were freed -- no leaks are possible
==7644== 
==7644== For counts of detected and suppressed errors, rerun with: -v
==7644== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
abhi@OptimusPrime:~/workspace/ExtraHop/src$ 



Future Works:
1. Hash Table can be made doubly link so that in order to print the result in the end 
   we can iterate through only the valid entries in the Hash. Although this is not 
   critical as assumption is size of the file is much bigger than hashtable and will
   take negligible time compared to file processing time.
2. Generic HashTable Implementation for supporting generic key-value pair on any data type.
3. Function pointer in Hash Init to  decide and control which HASH computation function needs
   to be called.A better hash compute function will have lesser collision.
