#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

  while ( 1 ) {
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; // get rid of newline

    char *command[1000];
    char *s = input;

    int i = 0;
    while ( s != NULL ) {
      command[i] = strsep(&s, " ");
      i++;
    }
    command[i] = NULL;
    execvp(command[0], command);
  }



  
  return 0;
}
