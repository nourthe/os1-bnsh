#include<stdio.h> //delete this.
#include<string.h>
#include<stdlib.h>

#include "builtin.h"

const char* func_name[FUNC_MAX] = {
	"exit",
	"quit",
	"cd",
	"echo",
	"help",
	//TODO clear
};

int get_func_argument_count(enum builtin_func func){
	switch(func){
		case EXIT:
		case QUIT:
		case HELP:
			return 0;
			break;
		case CD:
		case ECHO:
			return 1;
			break;
		//TODO clear
		default:
			//TODO error notice
			break;
	}
	return -1; // Not found
}
enum builtin_func find_builtin_function(char* func_string){
	for (int i = 0; i < FUNC_MAX; i++){
		if( strcmp(func_name[i], func_string) == 0){
			return i;
		}
	}

	return FUNC_MAX;
}

void exec_func(enum builtin_func func, int argc, char* argv[]){
	int func_arg_count = get_func_argument_count(func);
	if(func_arg_count == (argc - 1)){
		switch(func){
			case CD:
				//TODO CHANGE DIRECTORY
				printf("to-do: do change directory to: %s.\n", argv[1]);
				//
				break;
			case ECHO:
				printf("%s\n", argv[1]);
				break;
			case QUIT:
			case EXIT:
				printf("Exit by exit. Bye\n");
				exit(EXIT_SUCCESS);
				break;
			case HELP:
				printf("Estas ordenes de Shell están definidas internamente.\n");
				printf(" cd [dir]     \t\t\t Change working directory\n");
				printf(" help         \t\t\t Show this help message\n");
				printf(" exit         \t\t\t Finish the shell\n");
				break;
				//TODO clear
			default:
				//TODO error notice
				break;
		}
	}else{
		printf("bnsh: %s: Wrong number of arguments.\n", func_name[func]);
		//TODO argument error.
	}
}
