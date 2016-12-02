#include "util.h"

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
int execute(char *args[]);

// executes command w/ redirection in
void redirect_in(char *args[]);

// executes command w/ redirection out
void redirect_out(char *args[]);

// executes command w/ any combination of redirections
void redirect(char *args[]);

// executes command w/ piping out
void pipe_command(char *args[], int option);
//void pipe_command(char *args[]);

// updates .shell_history
void store_history(char *str, int num);

// displays history
void show_history();

// prints current working directory
void printdir();

#endif
