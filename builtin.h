#ifndef BUILTIN_H
#define BUILTIN_H

enum builtin_func{
	EXIT,
	QUIT,
	CD,
	ECHO,
	HELP,
	CLEAR,
	FUNC_MAX
};

int get_func_argument_count(enum builtin_func func);
enum builtin_func find_builtin_function(char* func_string);
void exec_func(enum builtin_func func, int argc,  char* args[]);
#endif
