#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*******************************************
EXECUTE: forks child to exec command, waits
* Input:
    > args: array of char pointers
        > args[0] is command
        > args[1] and on are args
	* Output: void
	********************************************/
void execute(char *args[]) {
  int f = fork();
  if ( f == 0 )
    //execlp(a, b, c);
    execvp(args[0], args);
  else
    wait(&f);
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


int main() {

  while ( 1 ) {
    printf(">>> ");
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

      if( !strcmp(command[0], "exit") ) {
	printf("Exiting shell...\n");
	exit(0);

      } else if ( !strcmp(command[0], "cd") ) {
	//do something here
	execvp(command[0], command);

      } else {
	execute(command);
      }

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
