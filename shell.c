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
    printf("%s\n", ret[i]);
    i++;
  }
  *ret[i] = 0;

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
    
    
    while ( t != NULL ) {
      //char *s=strsep(&t, ";");
      // another while loop to run individual
        printf("testttttt1\n");  
      char ** command = split(input, " ");
      break;
    /*
      if( (!strcmp(command[0], "exit")) || (!strcmp(command[0], "cd"))){
      execvp(command[0], command);
      }
    */
    //execute(command);

  }
  
return 0;
  }}
