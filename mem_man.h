#ifndef HEADER_FILE
#define HEADER_FILE

int main(int argc, char* argv[]);
void parse(char* line, char** args);
int isValidArgs(char** args);
int allocate(int pid, char* instruction, int v_address, int value);
int store(int pid, char* instruction, int v_address, int value);
int load(int pid, char* instruction, int v_address);

#endif