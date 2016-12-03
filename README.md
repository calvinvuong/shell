Arma-shmget-don
by Giovanni Topalini, Calvin Vuong, and William Xiang

## Features:
* Forks and executes commands
* Parses mutliple commands on one line separated by ;
* Redirects using >, <
  * Supports double redirection
* Pipes using |
  * Does not support multiple piping
* Command history listing with: $ history
  * Hidden history file to store used commands is created in the user home directory.
* Prints working directory preceding prompt
* Versatile whitespace parsing
  * Any amount of whitespace (or absence of it) is OK!

## Attempted:
* Multiple piping was attempted and almost successful, but problems occurred with the redirection and duplication of file descriptors that altered the behavior of multi-piping.
  * Code was not commented out because it does not break the program, but may temporarily alter the prompter.

## Bugs:
* Multiple pipe doesn't work work as intended
  * A | B | C will end up doing A | C
  * may temporarily alter directory prompt
  * exit may require pressing enter
* You may need to call exit more than once to exit the shell.
  * This might not be a bug, since sometimes it takes multiple exit commands to exit bash, but we don't really know.
* Redirection (<, >) cannot be used with pipes.

## Files & Function Headers:
### shell.c
Handles main command line functionality

	/*======== int execute() ==========
	Inputs:  char *args[]
	Returns: 1 if executed successfully, 0 otherwise

	char *args[]: array of command line argument strings
	
	Handles regular and custom (hardcoded) command cases
	Intended to be called right after command is split by space
	==================================*/

	/*======== void redirect() ==========
	Inputs:  char *args[]
	Returns: void

	*args[]: array of command line argument strs, which contains ">" or "<"

	helper for execute() when command contains
		redirection characters

	Handles regular command cases involving redirection
	Can handle double redirection
	Intended to be called when needed in execute()
	==================================*/

	/*======== void pipe_command() ==========
	Inputs:  int *args[], int option
	Returns: void

	*args[]: array of command line argument strs, which contains "|"
	option: 
	      0, if first cmd in args is the first cmd of pipe link
	      1, if first cmd in args is in the middle of pipe link
	      2, if first cmd in args is the last cmd of pipe link	

	Handles command execution if involving piping
	*** Do NOT use in conjunction with redirection
	==================================*/

	/*======== void store_history() ==========
	Inputs:  char *str, int num
	Returns: void

	str: command to be stored
	num: number entry of cmd (should be in chronorder)

	updates ~/.custom_shell_history_gt_cv_wx file
		that stores history of commands
	==================================*/

	/*======== void show_history() ==========
	Inputs:  void
	Returns: void

	prints contents of ~/.custom_shell_history_gt_cv_wx to stdout
	==================================*/

	/*======== void printdir() ==========
	Inputs:  void
	Returns: void

	prints contents of cwd, with the home address abbrievated with ~
	       (preceding before shell prompt)
	==================================*/

### util.c
Contains functions not specific to any one command task; e.g. splitting, parsing, etc.

	/*======== int find_str_in_array() ==========
	Inputs:  char *arr[], char str[]
	Returns: position of str in arr; -1 if not found

	*arr[]: an array of strings
	str[]: the string you are trying to find in array

	Looks for str in arr (an array of strings), and returns its position if present, -1 if not.
	==================================*/

	/*======== char ** split() ==========
	Inputs:  char * input, char * delimiter
	Returns: an array of string pointers, split by the delimiter

	*input: the string to be split
	*delimiter: the delimiter used to split the string

	Splits the input string into an array, each element separated by the specified delimiter.
	==================================*/

	/*======== char * whitespaceBeGone() ==========
	Inputs:  char * input
	Returns: a new string that is a copy of input, except with extra whitespaces removed

	*input: the string to strip of whitespace

	Condenses extra whitespace present in the input into single spaces in the return string
	==================================*/

	/*======== char * whitespaceBeHere() ==========
	Inputs:  char * input
	Returns: a new string that is a copy of input, except with spaces inserted appropriately

	*input: the string to be inserted with whitespace

	Inserts whitespace appropriately into a copy of input so that the return string can be parsed properly when split by space.
	==================================*/




	

	