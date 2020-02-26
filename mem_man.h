#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

//error codes
#define OUT_OF_BOUNDS 0x40
#define PAGE_OVERLAP 0x41
#define PAGE_LIMIT 0x42
#define MEM_FULL 0x43
#define INVALID_VAL 0x44
//...

typedef unsigned char addr;

int main(int argc, char* argv[]);
void parse(char* line, char** args);
int isValidArgs(char** args);
int allocate(int pid, char* instruction, addr v_address, uint8_t val);
int store(int pid, char* instruction, addr v_address, uint8_t val);
int load(int pid, char* instruction, addr v_address);
addr VPN_TO_MEM(int pid, addr address);
void err_handler(addr err, int err_val);

#endif