#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mem_man.h"

addr mem[64];
int free_list[4];//indices correspond to the pages, value stored at the index corresponds to the pid that's using the frame
				 //-1 means that the page is free 
addr proc_reg[4];//stores the physical address that points to the page table for the corresponding PID

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

int execute(int pid, char* instruction, addr v_address, int val) {
	printf("execute\n");
}

int allocate(int pid, char* instruction, addr v_address, int val) {
	//first we need an empty page frame
	int i;
	for(i = 0; i < 4; i++){

	}
	printf("allocate\n");
}

//returns the physical address at which the value is stored at
//returns -1 if failed
int store(int pid, char* instruction, addr v_address, int val) {
	addr phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x30){
		err_handler(phys_address, 0);
		return -1; 
	}
	mem[phys_address] = val;
	printf("Stored value %d at virtual address %d (physical address %d)\n", val, (unsigned int)v_address, (unsigned int)phys_address);
	return phys_address;
}

//returns the value stored in memory at the virtual address
//returns -1 if failed
int load(int pid, char* instruction, addr v_address) {
	addr phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x30){
		err_handler(phys_address, 0);
		return -1;
	}
	int val = mem[phys_address]; 
	printf("The value %d is at virtual address %d (physical address %d)\n", val, (unsigned int)v_address, (unsigned int)phys_address);
	return val;
}

//outputs the physical mem address of a given virtual address
addr VPN_TO_MEM(int pid, addr address){
	addr VPN  = address&0x30;//gets the VPN from the bits 4 and 5 from the virtual address
	addr offset = address&0x0f;//get the offset which are the 4 lower bits
	addr PTE = mem[proc_reg[pid]+VPN];//get the first PTE from the address of the page table stored in the proc reg
	if(!PTE) return OUT_OF_BOUNDS;//error code since the virtual address doesn't have a PTE which means it is out of bounds 
	addr PFN = PTE&0x30;//the physical frame numbers are located at bits 4 and 5
	return PFN+offset;//adds the PFN back to the offset to get the address in the frame that has the data
}

void err_handler(addr err, int err_val){
	switch(err){
		case OUT_OF_BOUNDS: 
			printf("ERROR: virtual address outside of page boundaries\n");
		case PAGE_OVERLAP:
			printf("ERROR: virtual page already mapped into physical frame %d\n", err_val);
		default: 
			printf("ERROR: unknown error, tried to access physical memory address %d, which is larger than memory\n", (unsigned int)err);
	}
}