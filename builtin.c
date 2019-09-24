#include<stdio.h> //delete this.
#include<string.h>
#include<stdlib.h>

#include "builtin.h"

const char* func_name[FUNC_MAX] = {
	"exit",
	"cd",
};

int get_func_argument_count(enum builtin_func func){
	switch(func){
		case EXIT:
			return 0;
			break;
		case CD:
			return 1;
			break;
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
			case EXIT:
				printf("\nExit by exit. Bye\n");
				exit(EXIT_SUCCESS);
				break;
			default:
				//TODO error notice
				break;
		}
	}else{
		printf("bnsh: %s: Wrong number of arguments.\n", func_name[func]);
		//TODO argument error.
	}
}
