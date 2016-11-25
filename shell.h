#ifndef SHELL_H
#define SHELL_H

// executes command using execvp
void execute(char *args[]);

// splits input str on given delimeter(s)
char ** split(char * input, char * delimiter);

// condenses whitespace into only one space
char * whitespaceBeGone(char * input);

#endif
