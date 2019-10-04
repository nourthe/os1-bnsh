#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<getopt.h> // get_long_options
#include<unistd.h> // gethostname
#include<limits.h> // PATH_MAX
#include<fcntl.h> // open, dup, dup2
#include <sys/wait.h> // waitpid

#include<signal.h> // Ctrl-C

#include"parser.h"
#include"builtin.h"

#ifndef VERSION
#define VERSION "unknown"
#endif

#define COMMAND_LINE_BUFFER_SIZE 2024

#define ONLY_A_NEW_LINE "ONLY_A_NEW_LINE"

// Terminal (UNIX?) ANSI Colors
#define COLOR_GREEN "\x1b[32m" // por que \e "no es standard"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

int main_loop(void);
void print_version(void);
void print_usage(void);
void get_prompt(char* buffer);
int read_line(char* buffer, int buf_size);
void cancel();
void invoke(char*, char**);

int main(int argc, char* argv[]){
  signal(SIGINT, cancel);
  const struct option long_opt [] = {
                                     {"help", no_argument, NULL, 1},
                                     {"version", no_argument, NULL, 'v'},
                                     {0, 0, 0, 0},
  };
  int c = 0; // getopt_long internal counter
  int opt_ind = 0; // option Index
  int show_help = 0;
  int show_version = 0;
  int error = 1;
  while ((c = getopt_long(argc, argv, "vc:", long_opt, &opt_ind))!=-1){
    switch(c){
    case 1:
      show_help = 1;
      break;
    case 'v':
      show_version = 1;
      break;
    case 'c':
      break;
    default:
      /* Params error */
      /* show_help = 1; */
      /* error = 1; */
      break;
   
      if(show_version && error==0 ){
        print_version();
        return 0;
      }
      if(show_help){
        print_usage();
        return error;
      }
    }
  }
  //TODO Si recibe un archivo como argumento, ejecutar cada linea de él.

  /* Main Loop */
  main_loop();
  return 0;
}
void print_usage(){
  printf("Usage: bnsh [option] | [shell file location]\n");
  printf("Opciones largas:\n");
  printf("\t--help\n");
  printf("\t--version:\n");
  printf("\t-c orden\t\t(solo invocación):\n");
  printf("\n");
  printf("Teclee `bash -c help` para ver las órdenes internas de la Shell.\n");
}

void print_version(){
  printf("bnsh, version %s\n", VERSION);
}

int main_loop(){
  int keep = 1;
  while(keep){
    char prompt[5000];
    get_prompt(prompt);
    printf("%s", prompt);

    char line[COMMAND_LINE_BUFFER_SIZE];
    read_line(line, COMMAND_LINE_BUFFER_SIZE);
    if(strcmp(line, ONLY_A_NEW_LINE)== 0){
      continue;
    }

    command_t* commands = new_node();
    parse_command(line, commands);

    /* Commands loop */
    command_t* command = commands;
    while(command != NULL){
      int mfunc = find_builtin_function(command->name);
      if ( !(mfunc == FUNC_MAX)){
        exec_func(mfunc, command->argc, command->argv);
        command = command->next;
        continue;
      }

      int background = 0;
      int stat_loc;
      // saving stdin and out to restore them later
      int tmpin = dup(0);
      int tmpout = dup(1);

      int fdin;
      if (command->input) {
        // use declared input
        fdin = open(command->input, O_RDONLY);
      } else {
        // or use default input
        fdin = dup(tmpin);
      }

      pid_t ret;
      int fdout;
      //
      // // EX COMMANDS LOOP
      //
      // an & given to any command in the chain is treated
      // as if given to the whole chain
      if (command->is_concurrent)
        background = 1;

      // redirect input
      dup2(fdin, 0);
      close(fdin);
      // setup output
      if (!command->is_piped) {
        // last piped command
        if (command->output) {
          fdout = open(command->output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        } else {
          // use default output
          fdout = dup(tmpout);
        }
      } else {
        // we are not in the last command yet
        // create pipe
        int fdpipe[2];
        pipe(fdpipe);
        fdout = fdpipe[1];
        fdin = fdpipe[0];
      }

      // redirect output
      dup2(fdout, 1);
      close(fdout);

      //create child process
      ret = fork();
      // if there was a problem forking
      if (ret < 0) {
        perror("fork\n");
        exit(EXIT_FAILURE);
      }
      if (ret == 0) {
        invoke(command->name, command->argv);
      }

      // restore input and output to default
      dup2(tmpin, 0);
      dup2(tmpout, 1);
      close(tmpin);
      close(tmpout);

      if (!background) {
        waitpid(ret, &stat_loc, WUNTRACED);
      }

      keep = 1;
      command = command->next;
    }
  }
  return keep;
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
  //TODO realizar un "continue" en el main-loop
  /*char prompt[5000];
    get_prompt(prompt);
    printf("\n%s", prompt);*/

  printf("\nExit by Ctrl-C.\n");
  exit(0);
}

void invoke(char *program, char **args) {
  // if there is an absolute or relative path, the program needn't be looked for
  if (program[0] == '/' || program[0] == '.') {
    if (execv(program, args) < 0)
      printf("baash: %s: no se encontró la orden.\n", program);
  } else {
    // search in $PATH
    char *paths = getenv("PATH");
    char *path;
    int status = 1;

    path = strtok(paths, ":");

    /*
     * Agrego program al final de cada entrada de $PATH y llamo a execv con eso para ver si
     * lo encuentra y lo puede ejecutar. Si no, va a retornar un -1 que va a cambiar status
     */
    while (path != NULL) {
      size_t sz = snprintf(NULL, 0, "%s/%s", path, program);
      char *buf = (char *)malloc(sz + 1);
      if (buf == NULL)
        exit(EXIT_FAILURE);

      snprintf(buf, sz+1, "%s/%s", path, program);

      // Si execv retorna, es porque no pudo ejecutar el programa
      if ((execv(buf, args) < 0))
        status = 0;

      path = strtok(NULL, ":");
      free(buf);
    }

    /*
     * Si al recorrer $PATH no se ejecutó el programa, intento ejecutarlo directamente por si
     * la direcció es relativa pero tiene el ./ implícito.
     * Ej: $HOME/SO1 $ ksamp/ksamp (equivalente a ./ksamp/ksamp)
     */
    if(!status)
      if ((execv(program, args) < 0))
        printf("baash: %s: no se encontró la orden.\n", program);

    free(paths);
    free(path);
  }
}
