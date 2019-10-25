#include"parser.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void parse_command(char* line, command_t* head){
	command_t *node = head;
	char *token;
	int pos = 0;

	token = strtok(line, ARGS_DELIM);
	int delim_required = 0;

	while (token != NULL) {
		char delim = is_delim(token);

		if (delim_required && delim < 0) {
			//TODO notice error, in this form?
			//perror("Syntax error.\n");
			exit(EXIT_FAILURE);
		}

		if (pos == 0 && delim < 0) {
			node->name = token;
		} else if (pos == 0 && delim >= 0 && delim != ';') {
			//TODO notice error, in this form? 
			//perror("Syntax error.\n");
			exit(EXIT_FAILURE);
		}

		if (delim < 0) {
			// get size of token
			// snprintf devuelve el tamaño del formateo incluso aunque se pase del limite
			// por eso se usa como 2 primeros argumentos 0
			size_t sz_token = snprintf(NULL, 0, "%s", token);
			// alloc memory to store it
			node->argv[pos] = (char *)malloc((sz_token + 1) * sizeof(char));
			// assign it
			sprintf(node->argv[pos], token);
			pos++;
			// increment number of arguments
			node->argc++;
		} else {
			switch (delim) {
				case '|':
					node->is_piped = 1;
					delim_required = 0;
					node->next = new_node();
					node = node->next;
					pos = 0;
					break;
				case '&':
					node->is_concurrent = 1;
					delim_required = 0;
					node->next = new_node();
					node = node->next;
					pos = 0;
					break;
				case '>':
					node->output = strtok(NULL, ARGS_DELIM);
					if (node->output == NULL)
						exit(EXIT_FAILURE);
					delim_required  = 1;
					break;
				case '<':
					node->input = strtok(NULL, ARGS_DELIM);
					if (node->input == NULL)
						exit(EXIT_FAILURE);
					delim_required = 1;
					break;
				default:
					pos = 0;
					delim_required = 0;
					node->next = new_node();
					node = node->next;
					break;
			} 
		}

		token = strtok(NULL, ARGS_DELIM);
	}
}
command_t* new_node(){
	command_t* temp; // declare a node
	temp = (command_t*)malloc(sizeof(command_t)); // allocate memory using malloc()
	// TODO free() left? free that memory somewhere
	if (temp == NULL){
		// TODO notice error
		exit(1);
	}
	temp->name = NULL;
	temp->argv = (char **)malloc(100*sizeof(char*));
	// TODO free() left? free that memory somewhere (again)
	temp->argc = 0;
	temp->is_piped = 0;
	temp->input = NULL;
	temp->output = NULL;
	temp->next = NULL;

	return temp;
}
char is_delim(char *string){
	//TODO ¿¿Crear la lista fija y calcular el size cada vez que se llama a la funcion??
	char *SPECIAL_CHARS[] = {"|", ">", "<", "&", ";"};
	int SPECIAL_CHARS_LEN = sizeof(SPECIAL_CHARS)/sizeof(SPECIAL_CHARS[0]);

	for (int i = 0; i < SPECIAL_CHARS_LEN; i++) {
		if (strcmp(string, SPECIAL_CHARS[i]) == 0)
			return SPECIAL_CHARS[i][0];
	}

	return -1;
}
