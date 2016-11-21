#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

  while ( 1 ) {
    printf(">>> ");
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; // get rid of newline

    char *commands[1000];
    char *t = input;


    while ( t != NULL ) {
      char *s=strsep(&t, ";");
      // another while loop to run individual
      char *command[1000];
	    
      int i = 0;
      while ( s != NULL ) {
	command[i] = strsep(&s, " ");
	i++;
      }
      command[i] = NULL;


    /*
    if( (!strcmp(command[0], "exit")) || (!strcmp(command[0], "cd"))){
      execvp(command[0], command);
    }
    */
    
      int f = fork();
      if (f == 0 ) {
	execvp(command[0], command);
      } else {
	wait(&f);
      }
    }

  }



  
  return 0;
}
