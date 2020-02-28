#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

//Page operations
#define WRITE 0
#define READ 1

//Page table entry bit fields
#define PROTEC_BIT 0x01 //protection bit stored in bit position 0, 1 = r/w mode, 0 = read only mode
#define VAL_BIT 0x02 //valid bit stored in bit position 1, 1 = page is in memory, 0 = page is on disk

//error codes
#define OUT_OF_BOUNDS 0x40
#define PAGE_OVERLAP 0x41
#define PAGE_LIMIT 0x42
#define MEM_FULL 0x43
#define INVALID_VAL 0x44
#define INVALID_WRITE 0x45
#define NO_PAGES 0x46
//...

//debug modes
#define RUN 1 //this is just to give the program instructions for normal operation
#define MEM_MAP 2
#define MEM_DSK_MAP 3
#define LIST_REG 4
#define LIST_PTE 5
#define FULL 6

//for displaying binary
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

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
void debugger(int mode);

#endif