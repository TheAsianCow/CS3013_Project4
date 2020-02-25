#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mem_man.h"

char mem[64];
int free_list[4];
char proc_reg[4];

int main(int argc, char* argv[]){
	int pid;
	char* instruction;
	int v_address;
	int val;

	// command prompt
	printf("Instruction? ");
	scanf("%d %s %d %d", &pid, instruction, &v_address, &val);

	printf("pid = %d, inst = %s, va = %d, val = %d\n", pid, instruction, v_address, val);
    execute(pid, instruction, v_address, val);
    return 0;
}

int execute(int pid, char* instruction, int v_address, int val) {
	printf("execute\n");
}

int allocate(int pid, char* instruction, int v_address, int val) {
	printf("allocate\n");
}

int store(int pid, char* instruction, int v_address, int val) {
	printf("store\n");
	char phys_address = VPN_TO_PFN(pid, v_address);
	mem[phys_address] = val;
	return 1;
}

int load(int pid, char* instruction, int v_address) {
	printf("load\n");
}

//outputs the physical address of a given virtual address, which basically just gives you the index of the array
//the first 2 bits of the array should always be 0, otherwise its automatically invalid since it exceeds the boundaries of the array
char VPN_TO_PFN(int pid, char address){
	char offset = address&0b00001111;//get the offset
	char PFN = proc_reg[pid];
	return PFN+offset;
}