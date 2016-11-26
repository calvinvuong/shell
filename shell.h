#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef SHELL_H
#define SHELL_H

// executes command using execvp
void execute(char *args[]);

// executes command w/ redirection in
void redirect_in(char *args[]);

// executes command w/ redirection out
void redirect_out(char *args[]);

// executes command w/ piping out
void pipe_command(char *args[]);

// updates .shell_history
void store_history(char *str, int num);

// displays history
void show_history();

// splits input str on given delimeter(s)
char ** split(char * input, char * delimiter);

// returns the pos of str in string array arr; -1 if does not exist
int find_str_in_array(char *arr[], char str[]);

// condenses whitespace into only one space
char * whitespaceBeGone(char * input);

#endif
