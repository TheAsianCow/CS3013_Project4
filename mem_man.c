#include "mem_man.h"

addr mem[64];
int free_list[4];//indices correspond to the pages, value stored at the index corresponds to the pid that's using the frame
				 //-1 means that the page is free 
addr proc_reg[4];//stores the physical address that points to the page table for the corresponding PID
				 //-1 means that the process doesn't have a page table

int main(int argc, char* argv[]){
	char pid_str[5];
	int pid;
	char* instruction;
	char v_address_str[5];
	// unsigned char v_address;
	char value_str[5];
	// int value;
	int correct_input = 0;
	// char* args[4];
	// char* v_ad_str;
	addr v_address;
	// char* value_str;
	uint8_t value;
	int num_args;
	// int correct_input = 0;
	int i;
	for(i = 0; i < 4; i++){
		free_list[i] = -1;
		proc_reg[i] = 0x80;
	}
	for(i = 0; i < 64; i++) mem[i] = 0x80;

	// char* file_path = "instructions.txt";
	char line[50];
    // char* line = array;
    char* args[50];
 //    ssize_t size;
 //    size_t n = 0;
 //    int eof = 0;

    // parsing
	// FILE* file = fopen(file_path,"r");
	// if(file == NULL) {
 //      perror("Error opening file");
 //      return(-1);
	// }
    // size = getline(&line,&n,file);
    // printf("size = %d\n", size);
    
   // if( fgets (str, 60, fp)!=NULL ) {
   //     writing content to stdout 
   //    puts(str);
   // }
    while(correct_input == 0){
    	printf("Instructions? ");
    	// scanf("%d %s %c %d\n", &pid, instruction, &v_address, &value);
    	fgets (line, 50, stdin);
    	num_args = parse(line, args);
    	// printf("line: %s\n", line);
    	// printf("%d\t%s\t%d\t%d\n", pid, instruction, v_address, value);
    	if (isValidArgs(args, num_args)) {
    		pid = atoi(args[0]);
    		instruction = args[1];
    		// printf("args 2 = %s\n", args[2]);
    		v_address = (unsigned char) atoi(args[2]);
    		value = atoi(args[3]);
    		if (strcmp(instruction, "allocate") == 0) allocate(pid, instruction, v_address, value);
			if (strcmp(instruction, "store") == 0) store(pid, instruction, v_address, value);
			if (strcmp(instruction, "load") == 0) load(pid, instruction, v_address);
			correct_input = 1;
    	}
    	else {
    		printf("Your arguments were incorrect.\nThe correct format is: (process id,instruction,virtual address,value)\n");
    		printf("\t- process id: int in range [0, 3]\n");
    		printf("\t- instruction: either \"allocate\", \"store\", or \"load\"\n");
    		printf("\t- virtual address: int in range [0, 63]\n");
    		printf("\t- value: int in range [0, 255]\n");
    	}
    }
    // fclose(file_path);
    return 0;
}

/*
 * Parses the given line and splits it into separate strings
 * whenever it sees a comma followed by a space.
 * @param line, the given line
 * @param args, where it stores the split strings
 */
int parse(char* line, char** args){
	// printf("parse\n");
	const char s[3] = ",";
	// const char n[2] = "\n";
	char* token = strtok(line, s);
	int argc = 0;
	while(token!=NULL){
		// printf("argc %d\ttoken = %s\n", argc, token);
		if (argc == 3) token[strlen(token) - 1] = '\0';
		args[argc] = strdup(token);
		argc++;
		token = strtok(NULL, s);
	}
	args[argc] = NULL;
	return argc;
}

int isValidArgs(char** args, int num_args) {
	int pid;
	char* instruction;
	addr v_address;
	uint8_t value;

	// make sure there's 4 args
	// printf("size = %d\n", num_args);
	if (num_args != 4) {
		// printf("not enough args\n");
		return 0;
	}
	// printf("first %d\n", isNumber(args[0]));
	// printf("second %d\n", isString(args[1]));
	// printf("third %d\n", isNumber(args[2]));
	// printf("fourth %d\n", isNumber(args[3]));
	// make sure the correct ones are numbers/strings
	if (isNumber(args[0]) && isString(args[1]) && isNumber(args[2]) && isNumber(args[3])) {
		pid = atoi(args[0]);
		instruction = args[1];
		// unsigned char v_address = (unsigned char) atoi(args[2]);
		// int value = atoi(args[3]);
		v_address = (unsigned char) atoi(args[2]);
		value = atoi(args[3]);
		// printf("isValidArgs\n");
		// printf("pid = %d\tinstruction = %s\tv_address = %d\tvalue = %d\n", pid, instruction, v_address, value);
		
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
	}
	// instruction = NULL;
	return 0;
}

int isNumber(char* mystery) {
	int length = strlen(mystery);
	// printf("is number size = %d\n", length);
	for (int i = 0; i < length; i++) {
		if (isdigit(mystery[i]) == 0) return 0;
	}
	return 1;
}

int isString(char* mystery) {
	int length = strlen(mystery);
	// printf("is string size = %d\n", length);
	for (int i = 0; i < length; i++) {
		if (isalpha(mystery[i]) == 0) return 0;
	}
	return 1;
}

int allocate(int pid, char* instruction, addr v_address, uint8_t val) {
	//first we need an empty page frame
	if(val!=1||val!=0){
		err_handler(INVALID_VAL, pid);
		return -1;
	}
	addr PFN = find_free();
	if(PFN>0x3f){
		err_handler(PFN, pid);
		return -1;
	}
	//now that we have a page that's free, we need to check if the process already has a page table
	addr page_table_addr = proc_reg[pid];
	if(page_table_addr==0x80){//process doesn't have a page table, so make one
		proc_reg[pid] = PFN;
		page_table_addr = proc_reg[pid];
		printf("Put page table for PID %d into physical frame %u\n", pid, (uint8_t)PFN);
		PFN = find_free();
		if(PFN>0x3f){
			err_handler(PFN, pid);
			return -1;
		}
	}
	addr VPN = v_address&0x30;
	addr PTE_offset = VPN>>4;
	addr PTE_addr = page_table_addr + PTE_offset;
	//now that we have the address to the page table, look at the entry corresponding to the virtual address and check if there's already an entry
	addr PTE = mem[PTE_addr];
	addr tmp_PFN = PTE&0x30;
	if(PTE==0x80){//this is a new entry
		mem[PTE_addr] = PFN+val;
		printf("Mapped virtual address %u (page %d) into physical frame %d\n", v_address, PTE_offset, PFN>>4);
	}
	else if(PTE&0x01==!val){//check the protection bit to see if its different from value
		mem[PTE_addr] = tmp_PFN+val;
		printf("Toggled protection bit to %d\n", val);
	}
	else err_handler(PAGE_OVERLAP,tmp_PFN>>4);//PFN already exists

	printf("allocate\n");
}

//returns the physical address at which the value is stored at
//returns -1 if failed
int store(int pid, char* instruction, addr v_address, uint8_t val) {
	addr phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x3f){
		err_handler(phys_address, 0);
		return -1; 
	}
	mem[phys_address] = (addr)val;
	printf("Stored value %u at virtual address %u (physical address %u)\n", val, (uint8_t)v_address, (uint8_t)phys_address);
	return phys_address;
}

//returns the value stored in memory at the virtual uaddress
//returns -1 if failed
int load(int pid, char* instruction, addr v_address) {
	addr phys_address = VPN_TO_MEM(pid, v_address);
	if(phys_address>0x3f){
		err_handler(phys_address, 0);
		return -1;
	}
	uint8_t val = (int)mem[phys_address]; 
	printf("The value %u is at virtual address %u (physical address %u)\n", val, (uint8_t)v_address, (uint8_t)phys_address);
	return val;
}

addr find_free(){
	int frame_num;
	addr PFN = MEM_FULL;//if we loop through the full list of free pages and can't find anything then memory is full, will be changed with swapping
	for(frame_num = 0; frame_num < 4; frame_num++) if(free_list[frame_num]==-1) return PFN = frame_num<<4;
	return PFN; 
}

//outputs the physical mem address of a given virtual address
addr VPN_TO_MEM(int pid, addr address){
	addr VPN  = address&0x30;//gets the VPN from the bits 4 and 5 from the virtual address
	addr offset = address&0x0f;//get the offset which are the 4 lower bits
	addr PTE_offset = VPN>>4;
	addr PTE = mem[proc_reg[pid]+PTE_offset];//get the first PTE from the address of the page table stored in the proc reg
	if(PTE!=0x80) return OUT_OF_BOUNDS;//error code since the virtual address doesn't have a PTE which means it is out of bounds 
	addr PFN = PTE&0x30;//the physical frame numbers are located at bits 4 and 5
	return PFN+offset;//adds the PFN back to the offset to get the address in the frame that has the data
}

void err_handler(addr err, int err_val){
	switch(err){
		case OUT_OF_BOUNDS: 
			printf("ERROR: virtual address outside of page boundaries\n");
		case PAGE_OVERLAP:
			printf("ERROR: virtual page already mapped into physical frame %d\n", err_val);
		case PAGE_LIMIT:
			printf("ERROR: process %d has hit the max amount of pages it can have\n", err_val);
		case MEM_FULL:
			printf("ERROR: memory is full, unable to allocate more pages\n");
		case INVALID_VAL:
			printf("ERROR: invalid value passed to allocation of process %d\n", err_val);
		default: 
			printf("ERROR: unknown error, tried to access physical memory address %u, which is larger than memory\n", err);
	}
}