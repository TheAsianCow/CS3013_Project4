#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char mem[64];
int free_list[4];
char proc_reg[4];

int main(int argc, char* argv[]){



	return 0;
}

//outputs the physical address of a function 
char VPN_to_PFN(int pid, char address){
	int offset = address&0b00001111;

}