#ifndef BUILTIN_H
#define BUILTIN_H

typedef struct command_t{
	char* name;
	int argc;
	char** argv;
	int is_concurrent;
	int is_piped;
	char* input;
	char* output;

	struct command_t* next;
} command_t;

void parse_command(char*, command_t*);

#endif
