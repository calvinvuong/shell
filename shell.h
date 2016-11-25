#ifndef SHELL_H
#define SHELL_H

// executes command using execvp
void execute(char *args[]);

// executes command w/ redirection in
void redirect_in(char *args[]);

// executes command w/ redirection out
void redirect_out(char *args[]);

// splits input str on given delimeter(s)
char ** split(char * input, char * delimiter);

// returns the pos of str in string array arr; -1 if does not exist
int find_str_in_array(char *arr[], char str[]);

// condenses whitespace into only one space
char * whitespaceBeGone(char * input);

#endif
