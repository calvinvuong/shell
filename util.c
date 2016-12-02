#include "util.h"

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
