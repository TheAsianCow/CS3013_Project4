#include "mem_man.h"

addr mem[64];
addr disk[320];
int free_list[4];//indices correspond to the pages, value stored at the index corresponds to the pid that's using the frame
				 //-1 means that the page is free 
addr proc_reg[4];//stores the physical address that points to the page table for the corresponding PID
				 //-1 means that the process doesn't have a page table

int main(int argc, char* argv[]){
	int pid;
	char* instruction;
	int correct_input = 0;
	addr v_address;
	uint8_t value;
	int num_args;
	int i;
	for(i = 0; i < 4; i++){
		free_list[i] = -1;
		proc_reg[i] = 0x80;
	}
	for(i = 0; i < 64; i++) mem[i] = 0x80;
	for (i = 0; i < 320; i++) disk[i] = 0x80;

	char line[50];
    char* args[50];

    // instructions
    printf("Welcome to Jerfy and Jyalu's virtual memory system!\n");
    printf("The program will prompt you to give it an instruction and execute it.\n");
    printf("It will continue doing so until you type in the string \"stop\".\n");
    printf("Each instruction you input must follow the format:\n");
    printf("\t(process_id,instruction,virtual_address,value)\n");
	printf("\tArguments should be separated by a single comma i.e. \",\"\n\n");

    while(1) {
	    while(correct_input == 0){
	    	printf("\nInstructions? ");
	    	fgets (line, 50, stdin);
	    	num_args = parse(line, args);
	    	if (isValidArgs(args, num_args)) {
	    		pid = atoi(args[0]);
	    		instruction = args[1];
	    		v_address = (unsigned char) atoi(args[2]);
	    		value = atoi(args[3]);
	    		if (strcmp(instruction, "allocate") == 0) allocate(pid, v_address, value);
				if (strcmp(instruction, "store") == 0) store(pid, v_address, value);
				if (strcmp(instruction, "load") == 0) load(pid, v_address);
				correct_input = 1;
	    	}
	    	else {
	    		printf("Your arguments were incorrect.\nThe correct format is: (process_id,instruction,virtual_address,value)\n");
	    		printf("\t- Arguments should be separated by a single comma i.e. \",\"\n");
	    		printf("\t- process id: int in range [0, 3]\n");
	    		printf("\t- instruction: either \"allocate\", \"store\", or \"load\"\n");
	    		printf("\t- virtual address: int in range [0, 63]\n");
	    		printf("\t- value: int in range [0, 255]\n");

	    		if (exit_mem_man() == 1) exit(1);
	    	}
	    }
	    correct_input = 0;
	    if (exit_mem_man() == 1) exit(1);
	}
    return 0;
}

/*
 * Checks if the user wants to exit the program.
 */
int exit_mem_man() {
	char line[20];
	printf("\nType \"stop\" to stop the program. Otherwise, hit the enter key. ");
	fgets (line, 20, stdin);
	// printf("length = %d\n", strlen(line));
	for (int i = 0; i < strlen(line); i++)
		line[i] = tolower(line[i]);
	// printf("lower word = %s\n", line);
	if (strcmp(line, "stop\n") == 0) return 1;
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
		if (argc == 3) token[strlen(token) - 1] = '\0';
		args[argc] = strdup(token);
		argc++;
		token = strtok(NULL, s);
	}
	args[argc] = NULL;
	// printf("argc = %d\n", argc);
	return argc;
}

/*
 * Checks to see whether the arguments given by the user are valid.
 * @param args, the arguments
 * @param num_args, the number of arguments given by the user
 * @param 1 if valid, 0 otherwise
 */
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
	if (isNumber(args[0]) && isString(args[1]) && isNumber(args[2]) && isNumber(args[3])) {
		pid = atoi(args[0]);
		instruction = args[1];
		v_address = (unsigned char) atoi(args[2]);
		value = atoi(args[3]);

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

/*
 * Determines if the given string is a number.
 * @param mystery, the given string
 * @return 1 if the string is a number, 0 otherwise
 */
int isNumber(char* mystery) {
	int length = strlen(mystery);
	// printf("is number size = %d\n", length);
	for (int i = 0; i < length; i++) {
		if (isdigit(mystery[i]) == 0) return 0;
	}
	return 1;
}

/*
 * Determines if the given string is a word.
 * @param mystery, the given string
 * @return 1 if the string is a word, 0 otherwise
 */
int isString(char* mystery) {
	int length = strlen(mystery);
	// printf("is string size = %d\n", length);
	for (int i = 0; i < length; i++) {
		if (isalpha(mystery[i]) == 0) return 0;
	}
	return 1;
}

/*
 * Tells the memory manager to allocate a physical page: it creates a mapping
 * in the page table between a virtual and physical address. The manager must
 * determine the appropriate virtual page number using the virtual address.
 * @param pid, the process id to allocate
 * @param v_address, the virtual address to use
 * @param val, either 0 or 1
 * 		if 1: the page is readable and writeable
 * 		if 0: readable only
 * @return 0 on success, -1 otherwise
 */
int allocate(int pid, addr v_address, uint8_t val) {
	// first we need an empty page frame
	if(val!=1&&val!=0){
		// printf("was given a value of %u\n", val);
		err_handler(INVALID_VAL, pid);
		return -1;
	}
	addr PFN = 0x80;
	//now that we have a page that's free, we need to check if the process already has a page table
	addr page_table_addr = proc_reg[pid];
	if(page_table_addr==0x80){//process doesn't have a page table, so make one
		PFN = find_free(pid);
		if(PFN>0x3f){
			err_handler(PFN, pid);
			return -1;
		}
		proc_reg[pid] = PFN;
		page_table_addr = proc_reg[pid];
		printf("Put page table for PID %d into physical frame %u\n", pid, PFN>>4);
	}
	addr VPN = v_address&0x30;
	addr PTE_offset = VPN>>4;
	addr PTE_addr = page_table_addr + PTE_offset;
	//now that we have the address to the page table, look at the entry corresponding to the virtual address and check if there's already an entry
	addr PTE = mem[PTE_addr];
	addr tmp_PFN = PTE&0x30;
	if(PTE==0x80){//this is a new entry
		PFN = find_free(pid);
		if(PFN>0x3f){
			err_handler(PFN, pid);
			return -1;
		}
		mem[PTE_addr] = PFN+val;
		printf("Mapped virtual address %u (page %d) into physical frame %d\n", v_address, PTE_offset, PFN>>4);
	}
	else if((PTE&0x01)!=(val&0x01)){//check the protection bit to see if its different from value
		mem[PTE_addr] = tmp_PFN+val;
		printf("Toggled protection bit to %d\n", val);
	}
	else err_handler(PAGE_OVERLAP,tmp_PFN>>4);//PFN already exists

	// printf("allocate\n");
	return 0;
}

/*
 * Instructs the memory manager to write the given value into the
 * physical memory location associated with the given virtual address,
 * performing translation and page swapping as necessary.
 * @param pid, the given process id
 * @param v_address, the given virtual address to write to
 * @param val, the given value to store
 * @return: 
 *		the physical address at which the value is stored at on success,
 * 		-1 if failed
 */
int store(int pid, addr v_address, uint8_t val) {
	addr phys_address = VPN_TO_MEM(pid, v_address, 0);
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
int load(int pid, addr v_address) {
	addr phys_address = VPN_TO_MEM(pid, v_address, 1);
	if(phys_address>0x3f){
		err_handler(phys_address, 0);
		return -1;
	}
	uint8_t val = (int)mem[phys_address]; 
	printf("The value %u is at virtual address %u (physical address %u)\n", val, (uint8_t)v_address, (uint8_t)phys_address);
	return val;
}

/*
 * Finds an empty page frame and returns it.
 */
addr find_free(int pid){
	int frame_num;
	addr PFN = MEM_FULL;//if we loop through the full list of free pages and can't find anything then memory is full, will be changed with swapping
	if (PFN == MEM_FULL) swap(pid);
	for(frame_num = 0; frame_num < 4; frame_num++){
		// printf("free_list at frame %d has value %d\n", frame_num, free_list[frame_num]);
		if(free_list[frame_num]==-1){
			// printf("page frame %d is free\n", frame_num);
			free_list[frame_num] = pid;
			return PFN = frame_num<<4;
		}
	}
	return PFN; 
}

/*
 * Pages out a page of its own choosing to disk and returns the
 * physical address of the page frame in physical memory that it
 * just freed.
 * @param pid
 */
addr swap(int pid) {
	// pick page to kick out - round robin
		// keep updating counter for which page to evict - 0 1 2 3
	// write the banished page to disk
		// memcpy((disk_addr, mem_addr, size));
	// update page table to record disk location for evicted page
		// update page table
		// mark present
		// update PFN of PTE
	// return address of now empty page frame in phys mem
	return 0;
}

/*
 * Returns the physical mem address of a given virtual address.
 */
addr VPN_TO_MEM(int pid, addr address, int op){
	addr VPN = address&0x30;//gets the VPN from the bits 4 and 5 from the virtual address
	addr offset = address&0x0f;//get the offset which are the 4 lower bits
	addr PTE_offset = VPN>>4;
	addr PTE = mem[proc_reg[pid]+PTE_offset];//get the first PTE from the address of the page table stored in the proc reg
	if(PTE==0x80) return OUT_OF_BOUNDS;//error code since the virtual address doesn't have a PTE which means it is out of bounds 
	if(!(PTE&0x01)&&(op==0)) return INVALID_WRITE;
	addr PFN = PTE&0x30;//the physical frame numbers are located at bits 4 and 5
	return PFN+offset;//adds the PFN back to the offset to get the address in the frame that has the data
}

void err_handler(addr err, int err_val){
	switch(err){
		case OUT_OF_BOUNDS: 
			printf("ERROR: virtual address outside of page boundaries\n");
			break;
		case PAGE_OVERLAP:
			printf("ERROR: virtual page already mapped into physical frame %d\n", err_val);
			break;
		case PAGE_LIMIT:
			printf("ERROR: process %d has hit the max amount of pages it can have\n", err_val);
			break;
		case MEM_FULL:
			printf("ERROR: memory is full, unable to allocate more pages\n");
			break;
		case INVALID_VAL:
			printf("ERROR: invalid value passed to allocation of process %d\n", err_val);
			break;
		case INVALID_WRITE:
			printf("ERROR: illegal write to a read-only page\n");
			break;
		default: 
			printf("ERROR: unknown error, tried to access physical memory address %u, which is larger than memory\n", err);
			break;
	}
}