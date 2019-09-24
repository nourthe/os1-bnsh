#include"parser.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h> //gethostname
#include<limits.h> //PATH_MAX

#include<signal.h>
#include"builtin.h"



#define COMMAND_LINE_BUFFER_SIZE 2024

// Terminal (UNIX?) ANSI Colors
#define COLOR_GREEN "\x1b[32m" // por que \e "no es standard"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

void get_prompt(char* buffer);
void read_line(char* buffer, int buf_size);
void cancel();

int main(int argc, char* argv[]){
	signal(SIGINT, cancel);
	int keep = 1;
	/* Main Loop */
	while(keep){
		char prompt[5000];
		get_prompt(prompt);
		printf("%s", prompt);

		char line[COMMAND_LINE_BUFFER_SIZE];
		read_line(line, COMMAND_LINE_BUFFER_SIZE);

		command_t *commands = new_node();
		parse_command(line, commands);

		//int mfunc = find_builtin_function("cd");
		//if ( !(mfunc == FUNC_MAX)){
			//exec_func(mfunc);
		//}
	}
	return 0;
}
void get_prompt(char* buffer){
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("Can't read current directory...");
            exit(1);
        }
	char *user = getenv("USER");
	char host[BUF_ARGS_SIZE];
	gethostname(host, BUF_ARGS_SIZE);

	sprintf(buffer,
		"%s%s@%s %s%s%s $ ",
		COLOR_BLUE,
		user,
		host,
		COLOR_GREEN,
		cwd,
		COLOR_RESET
	);
}
void read_line(char* buffer, int buf_size){
	size_t len= buf_size;
	if(getline(&buffer, &len, stdin) == -1){
		printf("%s\n", "Could not read line.");
	}
}
void cancel(){
	//TODO realizar un continue en el main-loop
	printf("\nExit by Ctrl-C.\n");
	exit(0);
}
