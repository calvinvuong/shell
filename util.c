#include "util.h"

/*======== int find_str_in_array() ==========
  Inputs:  char *arr[], char str[]
  Returns: position of str in arr; -1 if not found

  *arr[]: an array of strings
  str[]: the string you are trying to find in array

  Looks for str in arr (an array of strings), and returns its position if present, -1 if not.
  ==================================*/ 
int find_str_in_array(char *arr[], char str[]) {
  int i;
  for ( i = 0; arr[i] != NULL; i++ ) {
    if ( strcmp( str, arr[i] ) == 0 )
      return i;
  }
  return -1;
}


/*======== char ** split() ==========
  Inputs:  char * input, char * delimiter
  Returns: an array of string pointers, split by the delimiter

  *input: the string to be split
  *delimiter: the delimiter used to split the string

  Splits the input string into an array, each element separated by the specified delimiter.
  ==================================*/
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


/*======== char * whitespaceBeGone() ==========
  Inputs:  char * input
  Returns: a new string that is a copy of input, except with extra whitespaces removed

  *input: the string to strip of whitespace

  Condenses extra whitespace present in the input into single spaces in the return string
  ==================================*/
char * whitespaceBeGone( char * input ) {
  char * newStr = (char *) calloc(1, 1000 * sizeof(char));

  int i = 0;
  for (; input[i] != '\0'; i++) {
    if ( !(input[i] == ' ' && (strlen(newStr)==0 || input[i+1] == '\0' || input[i+1] == ' ')) ) {
      sprintf(newStr, "%s%c", newStr, input[i]);
    }
  }
  input[i] = 0; // terminating null
  return newStr;
}


/*======== char * whitespaceBeHere() ==========
  Inputs:  char * input
  Returns: a new string that is a copy of input, except with spaces inserted appropriately

  *input: the string to be inserted with whitespace

  Inserts whitespace appropriately into a copy of input so that the return string can be parsed properly when split by space.
  ==================================*/
char * whitespaceBeHere( char * input) {
  char * newStr = (char *) calloc(1, 1000 * sizeof(char));
  int i = 0;
  for (; input[i] != '\0'; i++) {
    if ( input[i] == '>' || input[i] == '<' || input[i] == '|' ){
      sprintf(newStr, "%s%c%c%c", newStr, ' ', input[i], ' ');
    } else {
      sprintf(newStr, "%s%c", newStr, input[i]);
    }
  }
  input[i] = 0;
  return newStr;
}
