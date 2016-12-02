#include "shell.h"

/*******************************************
EXECUTE: forks child to exec command, waits
* Input:
    > args: array of char pointers
        > args[0] is command
        > args[1] and on are args
	* Output: 1 if exit; 0 if not
	********************************************/
int execute(char *args[]) {
  // custom command handling
  if( !strcmp(args[0], "exit") ) { 
    printf("Exiting shell...\n");
    return 1;
    //    exit(0);
  }
  else if ( !strcmp(args[0], "cd") ) {
    chdir(args[1]);
    return 0;
  }
  else if ( !strcmp(args[0], "history") ) {
    show_history();
    return 0;
  }

  // regular command handling
  int f = fork();
  if ( f == 0 )  {

    if ( find_str_in_array(args, "<") != -1 || find_str_in_array(args, ">") != -1 ) {
      redirect(args);
      return 0;
    }
      
    else if ( find_str_in_array(args, "|") != -1 ) {
      pipe_command(args);
      return 0;
    }
    else
      execvp(args[0], args);
  }
  else
    wait(&f);
  
  return 0;
}

/***************************************************************************
REDIRECT: handles command execution if there is a redirection of input/output
          can handle multiple redirection symbols
* Input: args is an array of char pointers that represent commands
         the redirection character  is an element of args
* Output: nothing returned; command is excuted
****************************************************************************/
void redirect(char *args[]) {
  int output_arrow = find_str_in_array(args, ">");
  int input_arrow = find_str_in_array(args, "<");
  int first_arrow; // arrow in lowest index
  char * output_file_name;
  char * input_file_name;

  if ( input_arrow != -1 ) {
    input_file_name = args[input_arrow + 1];
    int fd_input = open(input_file_name, O_RDONLY);
    if ( fd_input == -1 ) { // file does not exist
      printf("%s: No such file\n", input_file_name);
      return;
    }
    dup2(fd_input, STDIN_FILENO); // redirect stdin to file_name
    close(fd_input);
  }
  else {
    input_file_name = NULL;
    first_arrow = output_arrow;
  }
  
  if ( output_arrow != -1 ) {
    output_file_name = args[output_arrow + 1];
    int fd_output = open(output_file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    dup2(fd_output, STDOUT_FILENO); // redirect stdout to file_name
    close(fd_output);
  }
  else {
    output_file_name = NULL;
    first_arrow = input_arrow;
  }
  
  // both arrows exist; find earliest arrow
  if ( output_arrow > -1 && input_arrow > -1 ) {
    if ( input_arrow < output_arrow )
      first_arrow = input_arrow;
    else
      first_arrow = output_arrow;
  }

  args[first_arrow] = 0; // null terminate
  execvp(args[0], args);
}
  
/*************************************************************************
PIPE_COMMAND: handles command execution if there is a piping of output
* Input: args is an array of char pointers that represent commands
         the pipe character | is an element of args
**************************************************************************/
/*
void pipe_command(char *args[]) {
  char **= find_str_in_array(args, "|"); // positon of | in args
  char ** cmd2 = &(args[pipe_pos + 1]);  
  args[pipe_pos] = 0; // null terminate

  char file_name[] = ".pipe_file.tmp";
  int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0600);  
  int STDOUT_FILENO_DUP = dup(STDOUT_FILENO);
  int STDIN_FILENO_DUP = dup(STDIN_FILENO);
  
  dup2(fd, STDOUT_FILENO); // redirect stdout to stdin
  close(fd);
  execute(args);
  dup2(STDOUT_FILENO_DUP, STDOUT_FILENO);
  
  fd = open(file_name, O_RDONLY);  
  dup2(fd, STDIN_FILENO); // redirect stdout to stdin
  close(fd);

  //if ( find_str_in_array(cmd2, "|") )
    execute(cmd2);
  dup2(STDIN_FILENO_DUP, STDIN_FILENO);
  unlink(file_name);
}
*/
// this works for 1 pipe
void pipe_command(char *args[]) {
  int pipe_pos = find_str_in_array(args, "|"); // positon of | in args
  char ** cmd2 = &(args[pipe_pos + 1]);  
  args[pipe_pos] = 0; // null terminate

  char file_name[] = ".pipe_file.tmp";
  int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0600);  
  int STDOUT_FILENO_DUP = dup(STDOUT_FILENO);
  int STDIN_FILENO_DUP = dup(STDIN_FILENO);
  
  dup2(fd, STDOUT_FILENO); // redirect stdout to stdin
  close(fd);
  execute(args);
  dup2(STDOUT_FILENO_DUP, STDOUT_FILENO);
  
  fd = open(file_name, O_RDONLY);  
  dup2(fd, STDIN_FILENO); // redirect stdout to stdin
  close(fd);

  //if ( find_str_in_array(cmd2, "|") )
  execute(cmd2);
  dup2(STDIN_FILENO_DUP, STDIN_FILENO);
  unlink(file_name);
}


/************************************************************
STORE_HISTORY: updates file ~/.custom_shell_history with str
* Input: str is the command to be stored in history
         num is the chronological command order
*************************************************************/
void store_history(char *str, int num) {
  char path[1000];
  sprintf(path, "%s/.custom_shell_history_gt_cv_wx", getenv("HOME"));
  int fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);

  // write command #
  char num_str[100];
  sprintf(num_str, "%d\t", num);
  write(fd, num_str, strlen(num_str));

  // write command
  write(fd, str, strlen(str));
  write(fd, "\n", strlen("\n"));

  close(fd);
}

/*************************************************************
SHOW_HISTORY: prints out history form ~/.custom_shell_history
**************************************************************/
void show_history() {
  char path[1000];
  sprintf(path, "%s/.custom_shell_history", getenv("HOME"));
  int fd = open(path, O_RDONLY);
  // do not need to check if file does not exist b/c history command creates file anyways

  struct stat buf;
  stat(path, &buf);
  int size = buf.st_size;

  char contents[size+1];
  read(fd, contents, size);
  contents[size] = 0; // null terminate
  
  printf("%s\n",  contents);

}

// finds if specified string is in a null-terminated  array of strings
// returns pos if there; -1 if does not exist
int find_str_in_array(char *arr[], char str[]) {
  int i;
  for ( i = 0; arr[i] != NULL; i++ ) {
    if ( strcmp( str, arr[i] ) == 0 )
      return i;
  }
  return -1;
}
   
/******************************************
SPLIT: splits input str on given delimiter
* Input:
    > input: string to parse
    > delimiter: i wonder what this is
    * Output:
    > "array" of pointers to split strings 
*******************************************/
char ** split(char * input, char * delimiter) {
  char ** ret = (char **) malloc(1000);
  char *t = input;

  int i = 0;
  while ( t != NULL ) {
    ret[i] = strsep(&t, delimiter);
    i++;
  }
  ret[i] = 0; //null term for both exec and cmd parsing
  return ret;
}


/*************************************************
WHITESPACE-BEGONE: condenses whitespace in string
* Input:
    > input: string to condence
    * Output:
    > "array" of pointers to split strings 
**************************************************/
char * whitespaceBeGone( char * input ) {
  char * newStr = (char *) malloc(1000 * sizeof(char));

  int i = 0;
  for (; input[i] != '\0'; i++) {
    if ( !(input[i] == ' ' && (strlen(newStr)==0 || input[i+1] == '\0' || input[i+1] == ' ')) ) {
      sprintf(newStr, "%s%c", newStr, input[i]);
    }
  }
  return newStr;
}

/************************************************
comment later
*************************************************/
char * whitespaceBeHere( char * input) {
  char * newStr = (char *) malloc(1000 * sizeof(char));
  int i = 0;
  for (; input[i] != '\0'; i++) {
    if ( input[i] == '>' || input[i] == '<' || input[i] == '|' ){
      sprintf(newStr, "%s%c%c%c", newStr, ' ', input[i], ' ');
    } else {
      sprintf(newStr, "%s%c", newStr, input[i]);
    }
  }
  return newStr;
}


/**************************************************************************
PRINTDIR: prints the current working directory as seen in the shell prompt
**************************************************************************/
void printdir() {
  char dir[1000];  
  getcwd(dir, 1000);

  if ( strstr(dir, getenv("HOME")) == 0 ) // if cwd is above home dir
    printf("%s$ ", dir);
  else {
    printf("~%s$ ", strstr(dir, getenv("HOME")) + strlen(getenv("HOME")));
  }
}


int main() {
  int command_num = 0;
  int status = 0;
  
  while ( 1 ) {
    //printf(">>> ");
    printdir();
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; //get rid of newline

    // stor4 history
    store_history(input, command_num);
    command_num++;
    
    //list of commands separated by semicolons
    //char **commands = (char **) malloc(1000);
    char ** commands = split(input, ";");
    
    int i;
    for (i = 0; commands[i] != NULL; i++) { 
      char * command_nonsplit_nonWhitespaceBeGoned = whitespaceBeHere( commands[i] );
      char * command_nonsplit = whitespaceBeGone( command_nonsplit_nonWhitespaceBeGoned );
      char ** command = split(command_nonsplit, " ");

      /*      
	printf("\tENTIRE CMD: (%s)\n", command_nonsplit);
	printf("\tCOMMAND: (%s)\n", command[0]);
	printf("\tARG 1: (%s)\n", command[1]);
	printf("\tARG 2: (%s)\n", command[2]);
	printf("\tARG 3: (%s)\n", command[3]);
      */

      /*
	if( !strcmp(command[0], "exit") ) {
	printf("Exiting shell...\n");
	exit(0);

	} else if ( !strcmp(command[0], "cd") ) {
	chdir(command[1]);
	} else {
	execute(command);
	}
      */
      
      status = execute(command);
      free(command_nonsplit_nonWhitespaceBeGoned);
      free(command_nonsplit);
      free(command);
      //command_nonsplit_nonWhitespaceBeGoned = NULL;
      //command_nonsplit = NULL;
      //command = NULL;
      
    }  

    free(commands);
    //commands = NULL;
    if ( status == 1 )
      exit(0);
  }

  return 0;
}
