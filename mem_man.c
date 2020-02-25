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
	// char* pid_str;
	int pid;
	char* instruction;
	// char* v_ad_str;
	unsigned char v_address;
	// char* value_str;
	int value;
	// int correct_input = 0;

	char* file_path = "instructions.txt";
	char array[50];
    char* line = array;
    char* args[50];
    ssize_t size;
    size_t n = 0;
    int eof = 0;

    // parsing
	FILE* file = fopen(file_path,"r");
	if(file == NULL) {
      perror("Error opening file");
      return(-1);
	}
    // size = getline(&line,&n,file);
    // printf("size = %d\n", size);
    
   // if( fgets (str, 60, fp)!=NULL ) {
   //     writing content to stdout 
   //    puts(str);
   // }
    while(eof == 0){
    	// printf("line = %s\n", line);
        // if(line[size-1]=='\n') line[size-1]='\0';
        // printf("while\n");
        // size = getline(&line, &n, file);
        // execute(line);
        // size = getline(&line,&n,file);
        if (fgets(line, 50, file) != NULL){
	        printf("line = %s\n", line);
	        // split line into words and clean commas
	        parse(line, args);
	        // printf("%s\t%s\t%s\t%s\n", args[0], args[1], args[2], args[3]);
	        // check if words are valid
	        if (isValidArgs(args)) {
	        	pid = atoi(args[0]);
	        	instruction = args[1];
	        	strcpy((char*) args[2], instruction);
	        	value = atoi(args[3]);

	        	printf("pid = %d\n", pid);
	        	printf("instruction = %s\n", instruction);
	        	printf("v_address = %d\n", v_address);
	        	printf("value = %d\n", value);

	        	if (strcmp(instruction, "allocate") == 0) allocate(pid, instruction, v_address, value);
				if (strcmp(instruction, "store") == 0) store(pid, instruction, v_address, value);
				if (strcmp(instruction, "load") == 0) load(pid, instruction, v_address);
	        }
	    }
	    else
	    	eof = 1;
    }
    // fclose(file_path);
    return 0;
}

/*
 * Parses the given line and splits it into separate strings
 * whenever it sees a space.
 * @param line, the given line
 * @param args, where it stores the split strings
 */
void parse(char* line, char** args){
     const char s[3] = ", ";
     char* token = strtok(line, s);
     int argc = 0;
     while(token!=NULL){
        args[argc] = strdup(token);
        argc++;
        token = strtok(NULL, s);
     }
     args[argc] = NULL;
}

int isValidArgs(char** args) {
	int pid = atoi(args[0]);
	char* instruction = args[1];
	unsigned char v_address = (int) args[2];
	int value = atoi(args[3]);
	// printf("isValidArgs\n");
	// printf("%d\t%s\t%d\t%d\n", pid, instruction, v_address, value);
	// printf(pid >= 0 && pid <= 3);
	// printf(v_address >= 0 && v_address <= 63);
	// printf()

	if (pid >= 0 && pid <= 3 && v_address >= 0 && v_address <= 63 && value >= 0 && value <= 255) {
		// printf("true\n");
		if (strcmp(instruction, "allocate") == 0) return 1;
		if (strcmp(instruction, "store")) return 1;
		if (strcmp(instruction, "load")) return 1;
	}
	// instruction = NULL;
	return 0;
}

int allocate(int pid, char* instruction, unsigned char v_address, int val) {
	return 0;
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