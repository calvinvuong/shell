#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "shell.h"
/*******************************************
EXECUTE: forks child to exec command, waits
* Input:
    > args: array of char pointers
        > args[0] is command
        > args[1] and on are args
	* Output: void
	********************************************/
void execute(char *args[]) {
  // custom command handling
  if( !strcmp(args[0], "exit") ) { 
      printf("Exiting shell...\n");
      exit(0);
    }
  else if ( !strcmp(args[0], "cd") ) {
    chdir(args[1]);
    return;
  }

  // regular command handling
  int f = fork();
  if ( f == 0 )  {
    
    if ( find_str_in_array(args, ">") != -1 )
      redirect_out(args);
    else if ( find_str_in_array(args, "<") != -1 )
      redirect_in(args);
    else
      execvp(args[0], args);
  }
  else
    wait(&f);
}

void redirect_out(char *args[]) {
  int arrow_pos = find_str_in_array(args, ">"); // position of > in args
  char *file_name = args[ arrow_pos + 1 ];
  
  int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  dup2(fd, 1); // redirect sdout to file_name
  close(fd);

  args[arrow_pos] = 0; // null terminate
  execvp(args[0], args);
  
}


void redirect_in(char *args[]) {};

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

/**************************************************************************
PRINTDIR: prints the current working directory as seen in the shell prompt
**************************************************************************/
void printdir() {
  char username[100];
  char dir[500];
  //char short_dir[500]; // uses ~ as an abbreivation
  
  getlogin_r(username, 100); // puts login name in username
  getcwd(dir, 500);

  if ( strstr(dir, username) == 0 ) // cwd is above home dir
    printf("%s$ ", dir);
  else {
    printf("~/%s$ ", strstr(dir, username));
  }
}


int main() {

  while ( 1 ) {
    //printf(">>> ");
    printdir();
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; //get rid of newline

    //list of commands separated by semicolons
    char **commands = (char **) malloc(1000);
    commands = split(input, ";");
 
    int i;
    for (i = 0; commands[i] != NULL; i++) { 
      char * command_nonsplit = (char *) malloc(1000);
      char ** command = (char **) malloc(1000);

      command_nonsplit = whitespaceBeGone( commands[i] );
      command = split(command_nonsplit, " ");

      /*
	printf("\tENTIRE CMD: (%s)\n", command_nonsplit);
	printf("\tCOMMAND: (%s)\n", command[0]);
	printf("\tARG 1: (%s)\n", command[1]);
	printf("\tARG 2: (%s)\n", command[2]);
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

      execute(command);
      
      free(command_nonsplit);
      free(command);
      command_nonsplit = NULL;
      command = NULL;
    }  

    free(commands);
    commands = NULL;
  }

  return 0;
}
