#ifndef BUILTIN_H
#define BUILTIN_H

// ARGS
#define BUF_ARGS_SIZE 1024
#define ARGS_DELIM " \t\r\n\a"

/* Linked List Node */
typedef struct command_struct{
	char* name;
	int argc;
	char** argv;
	int is_concurrent;
	int is_piped;
	char* input;
	char* output;

	struct command_struct* next;
} command_t;

void parse_command(char*, command_t*);
/* Linked List init */
command_t* new_node(void);

char is_delim(char *string);

#endif
