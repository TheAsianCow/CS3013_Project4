#ifndef HEADER_FILE
#define HEADER_FILE

int main(int argc, char* argv[]);
void parse(char* line, char** args);
int isValidArgs(char** args);
int execute(int pid, char* instruction, unsigned char v_address, int val);
int allocate(int pid, char* instruction, unsigned char v_address, int val);
int store(int pid, char* instruction, unsigned char v_address, int val);
int load(int pid, char* instruction, unsigned char v_address);
unsigned char VPN_TO_MEM(int pid, unsigned char address);
void err_handler(unsigned char err);

#endif