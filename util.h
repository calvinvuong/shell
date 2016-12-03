#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef UTIL_H
#define UTIL_H
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// splits input str on given delimeter(s)
char ** split(char * input, char * delimiter);

// returns the pos of str in string array arr; -1 if does not exist
int find_str_in_array(char *arr[], char str[]);

// condenses whitespace into only one space
char * whitespaceBeGone(char * input);

// comment later
char * whitespaceBeHere(char * input);
#endif
