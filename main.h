#ifndef MAIN_H
#define MAIN_H


enum builtin_func{
	EXIT,
	CD,
	2
	//FUNC_MAX
};
int get_func_argument_count(enum builtin_func func);
enum builtin_func find_builtin_function(char* func_string);
void exec_func(enum builtin_func func, ...);
#endif
