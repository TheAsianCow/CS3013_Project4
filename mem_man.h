#ifndef HEADER_FILE
#define HEADER_FILE

//error codes
#define OUT_OF_BOUNDS 0x40
#define PAGE_OVERLAP 0x41
#define ERR3 0x42
//...

typedef unsigned char addr;

int main(int argc, char* argv[]);
void parse(char* line, char** args);
int isValidArgs(char** args);
int allocate(int pid, char* instruction, addr v_address, int val);
int store(int pid, char* instruction, addr v_address, int val);
int load(int pid, char* instruction, addr v_address);
addr VPN_TO_MEM(int pid, addr address);
void err_handler(addr err, int err_val);

#endif