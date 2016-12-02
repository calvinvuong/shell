#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef UTIL_H
#define UTIL_H

// splits input str on given delimeter(s)
char ** split(char * input, char * delimiter);

// returns the pos of str in string array arr; -1 if does not exist
int find_str_in_array(char *arr[], char str[]);

// condenses whitespace into only one space
char * whitespaceBeGone(char * input);

// comment later
char * whitespaceBeHere(char * input);
#endif
