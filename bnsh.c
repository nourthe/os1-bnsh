#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "builtin.h"

#define COMMAND_LINE_BUFFER_SIZE 2024

void get_prompt(char* buffer);
void read_line(char* buffer, int buf_size);

int main(int argc, char* argv[]){
	int keep = 1;
	while(keep){
		char prompt[512];
		get_prompt(prompt);
		printf("%s", prompt);

		char command[COMMAND_LINE_BUFFER_SIZE];
		read_line(command, COMMAND_LINE_BUFFER_SIZE);
	}
	return 0;
}
void get_prompt(char* buffer){
	char* ps1 = getenv("PS1");
	strcpy(buffer, ps1);
	strcat(buffer, "Fake Prompt $");
}
void read_line(char* buffer, int buf_size){
	size_t len= buf_size;
	if(getline(&buffer, &len, stdin) == -1){
		printf("%s\n", "Could not read line.");
	}
}
