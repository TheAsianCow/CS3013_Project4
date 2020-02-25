#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mem_man.h"

//error codes
#define OUT_OF_BOUNDS 0x40
#define ERR2 0x41
#define ERR3 0x42
//...

unsigned char mem[64];
int free_list[4];
unsigned char proc_reg[4];//stores 

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

int execute(int pid, char* instruction, unsigned char v_address, int val) {
	printf("execute\n");
}

int allocate(int pid, char* instruction, unsigned char v_address, int val) {
	printf("allocate\n");
}

//returns the physical address at which the value is stored at
//returns 0 if failed
int store(int pid, char* instruction, unsigned char v_address, int val) {
	unsigned char phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x30){
		err_handler(phys_address);
		return -1; 
	}
	mem[phys_address] = val;
	printf("Stored value %d at virtual address %d (physical address %d)\n", val, (int)v_address, (int)phys_address);
	return phys_address;
}

//returns the value stored in memory at the virtual address
int load(int pid, char* instruction, unsigned char v_address) {
	unsigned char phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x30){
		err_handler(phys_address);
		return -1;
	}
	int val = mem[phys_address]; 
	printf("The value %d is at virtual address %d (physical address %d)\n", val, (int)v_address, (int)phys_address);
	return val;
}

//outputs the physical mem address of a given virtual address
unsigned char VPN_TO_MEM(int pid, unsigned char address){
	unsigned char VPN  = address&0x30;//gets the VPN from the bits 4 and 5 from the virtual address
	unsigned char offset = address&0x0f;//get the offset which are the 4 lower bits
	unsigned char PTE = mem[proc_reg[pid]+VPN];//get the first PTE from the address of the page table stored in the proc reg
	if(!PTE) return OUT_OF_BOUNDS;//error code since the virtual address doesn't have a PTE which means it is out of bounds 
	unsigned char PFN = PTE&0x30;//the physical frame numbers are located at bits 4 and 5
	return PFN+offset;//adds the PFN back to the offset to get the address in the frame that has the data
}

void err_handler(unsigned char err){
	switch(err){
		case OUT_OF_BOUNDS: 
			printf("ERROR: virtual address outside of page boundaries\n");
		default: 
			printf("ERROR: unknown error, tried to access physical memory address %d, which is larger than memory\n", (unsigned int)err);
	}
}