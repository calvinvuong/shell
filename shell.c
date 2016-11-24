#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/**************************************
EXECUTE: forks child to exec command, waits
* Input:
    > args: array of char pointers
        > args[0] is command
        > args[1] and on are args
* Output: void
**************************************/
void execute(char *args[]) {
  int f = fork();
  if ( f == 0 )
    //execlp(a, b, c);
    execvp(args[0], args);
  else
    wait(&f);
}



/**************************************
SPLIT: splits input str on given delimiter
* Input:
    > input: string to parse
    > delimiter: i wonder what this is
* Output:
    > "array" of pointers to split strings 
**************************************/
char ** split(char input[], char delimiter[]) {
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



int main() {

  while ( 1 ) {
    printf(">>> ");
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; //get rid of newline

    char **commands = (char **) malloc(1000);  //list of commands separated by semicolons
    char *t = input;
    commands = split(input, ";");
 
    int i;
    for (i = 0; commands[i] != NULL; i++) { 
      
      char ** command = split(commands[i], " ");
      
      if( !strcmp(command[0], "exit") ) {
	printf("Exiting shell...\n");
	exit(0);

      } else if ( !strcmp(command[0], "cd") ) {
	//do something here
	execvp(command[0], command);

      } else {
	printf("COMMAND: %s\n", command[0]);
	printf("FIRST ARG: %s\n", command[1]);
	execute(command);
      }
    }  
  }

  //FREE STUFF LATER

  return 0;
}
