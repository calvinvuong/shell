#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


// takes an array of parsed arguments
// forks a child process to execute these arguments, waits
void execute(char *args[]) {
  int f = fork();
  if ( f == 0 )
    //execlp(a, b, c);
    execvp(args[0], args);
  else
    wait(&f);
}


/*
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
  *ret[i] = 0;

  return ret;

}
*/

/*SPACE BUG*/    
int main() {

  while ( 1 ) {
    printf(">>> ");
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; // get rid of newline

    char *commands[1000]; // list of commands separated by semicolons
    char *s = input;
    // split commands by semicolon
    int i = 0;
    while ( s != NULL ) {
      commands[i] = strsep(&s, ";");
      i++;
    }
    commands[i] = NULL;

    // for each command in list of commands
    int j = 0;
    for (; commands[j] != NULL; j++) {
      
      char * command[1000];
      char *t = commands[j];
      // split args by spaces
      int k = 0;
      while ( t != NULL ) {
	command[k] = strsep(&t, " ");
	k++;
      }
      command[k] = NULL;

      int l;
      for ( l = 0; command[l] != NULL; l++ )
	printf("%d, ", *command[l]);
      printf("\n");
      /*
      // do not pass leading spaces
      int l;
      for ( l = 0; command[l] == NULL; l++ );
      execute(command);
      */
    } 
  }
  
  return 0;
}




//char ** command = split(input, " ");
    /*
      if( (!strcmp(command[0], "exit")) || (!strcmp(command[0], "cd"))){
      execvp(command[0], command);
      }
    */
    //    execute(command);

