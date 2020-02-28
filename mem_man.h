#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

//error codes
#define OUT_OF_BOUNDS 0x40
#define PAGE_OVERLAP 0x41
#define PAGE_LIMIT 0x42
#define MEM_FULL 0x43
#define INVALID_VAL 0x44
#define INVALID_WRITE 0x45
//...

typedef unsigned char addr;

int main(int argc, char* argv[]);
int exit_mem_man();
int parse(char* line, char** args);
int isValidArgs(char** args, int num_args);
int isNumber(char* mystery);
int isString(char* mystery);
int allocate(int pid, addr v_address, uint8_t val);
int store(int pid, addr v_address, uint8_t val);
int load(int pid, addr v_address);
addr find_free(int pid);
addr swap(int page_ID);
addr evict(int add_page_ID);
addr VPN_TO_MEM(int pid, addr address, int op);
void err_handler(addr err, int err_val);

#endif