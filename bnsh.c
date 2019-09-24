#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h> // gethostname
#include<limits.h> // PATH_MAX

#include<signal.h> // Ctrl-C

#include"parser.h"
#include"builtin.h"

#define COMMAND_LINE_BUFFER_SIZE 2024

#define ONLY_A_NEW_LINE "ONLY_A_NEW_LINE"

// Terminal (UNIX?) ANSI Colors
#define COLOR_GREEN "\x1b[32m" // por que \e "no es standard"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

void get_prompt(char* buffer);
int read_line(char* buffer, int buf_size);
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

		command_t* commands = new_node();
		parse_command(line, commands);

		/* Commands loop */
		command_t* command = commands;
		while(command != NULL){
			int mfunc = find_builtin_function(command->name);
			if ( !(mfunc == FUNC_MAX)){
				exec_func(mfunc, command->argc, command->argv);
			}
			command = command->next;
		}
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
int read_line(char* buf, int buf_size){
	char *buffer;
	size_t bufsize = 32;
	size_t characters;
	buffer = (char *)malloc(bufsize * sizeof(char));
	if( buffer == NULL) {
		perror("Unable to allocate buffer");
		exit(1);
	}

	characters = getline(&buffer,&bufsize,stdin);
	if(characters < 1){ /*TODO notice error*/ }
	else if(characters == 1){
		//TODO better solution.
		strcpy(buf, ONLY_A_NEW_LINE);
	}
	else{ strcpy(buf, buffer); }

	//TODO Ya puede volver a retornar void
	return 0;
}
void cancel(){
	//TODO realizar un continue en el main-loop
	/*char prompt[5000];
	get_prompt(prompt);
	printf("\n%s", prompt);*/

	printf("\nExit by Ctrl-C.\n");
	exit(0);
}
