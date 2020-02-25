#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mem_man.h"

char mem[64];
int free_list[4];
char proc_reg[4];

int main(int argc, char* argv[]){
	// char* pid_str;
	int pid;
	char* instruction;
	// char* v_ad_str;
	int v_address;
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
	        	v_address = atoi(args[2]);
	        	value = atoi(args[3]);

	        	// printf("pid = %d\n", pid);
	        	// printf("instruction = %s\n", instruction);
	        	// printf("v_address = %d\n", v_address);
	        	// printf("value = %d\n", value);

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
	int v_address = atoi(args[2]);
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

int allocate(int pid, char* instruction, int v_address, int value) {
	printf("allocate\n");
}

int store(int pid, char* instruction, int v_address, int value) {
	printf("store\n");
}

int load(int pid, char* instruction, int v_address) {
	printf("load\n");
}

//outputs the physical address of a function 
char VPN_to_PFN(int pid, char address){
	int offset = address&0b00001111;
}