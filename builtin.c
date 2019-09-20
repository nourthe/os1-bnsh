#include <string.h>
#include <stdlib.h>

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
	if(func_arg_count == argc){
		switch(func){
			case CD:
				//TODO CHANGE DIRECTORY
				break;
			case EXIT:
				exit(EXIT_SUCCESS);
				break;
			default:
				//TODO error notice
				break;
		}
	}else{
		//TODO argument error.
	}
}
