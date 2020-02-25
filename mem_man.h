#ifndef HEADER_FILE
#define HEADER_FILE

int main(int argc, char* argv[]);
int execute(int pid, char* instruction, int v_address, int val);
int allocate(int pid, char* instruction, int v_address, int val);
int store(int pid, char* instruction, int v_address, int val);
int load(int pid, char* instruction, int v_address);

#endif