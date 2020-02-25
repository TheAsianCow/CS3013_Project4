#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mem_man.h"

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
}

int load(int pid, char* instruction, int v_address) {
	printf("load\n");
}