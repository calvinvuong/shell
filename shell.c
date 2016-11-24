#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


// takes an array of parsed arguments
// forks a child process to execute these arguments, waits
void execute(char *args[]) {
  int f = fork();
  if ( f == 0 )
    execvp(args[0], args);
  else
    wait(&f);
}



// takes a string of characters
// splits them based on a given delimiter
// returns an array of split strings
char ** split(char input[], char delimiter[]) {
  char ** ret = (char **) malloc(1000);
  char *t = input;

  int i = 0;
  while ( t != NULL ) {
    ret[i] = strsep(&t, delimiter);
    i++;
  }

  ret[i] = 0; //null term for exec

  return ret;
}


    
int main() {

  while ( 1 ) {
    printf(">>> ");
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; // get rid of newline

    char *commands[1000];
    char *t = input;
        
      //char *s=strsep(&t, ";");
      // another while loop to run individual
      char ** command = split(input, " ");
    
      if( !strcmp(command[0], "exit") ) {
	printf("Exiting shell...\n");
	exit(0);

      } else if ( !strcmp(command[0], "cd") ) {
	//do sometihng here
	execvp(command[0], command);

      } else {
	execute(command);
      }


    }  
    return 0;
  }
