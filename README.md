Arma-shmget-don
by Giovanni Topalini, Calvin Vuong, and William Xiang

Features:
	* Forks and executes commands
	* Parses mutliple commands on one line
	* Redirects using >, <
	    * Supports double redirection
	* Pipes using |
	* Command history listing with: $ history
	* Prints working directory preceding prompt
	* Versatile whitespace parsing
	    * Any amount of whitespace (or absence of it) is OK!

Attempted:

Bugs:
	* Multiple pipe doesn't work work as intended
	    * A | B | C will end up doing A | C
	* may mess up directory prompt, exit may require pressing entering (undefined behavior in general)



Files & Function Headers:
shell.c
	Handles main cmdline functionality

	/*======== int execute() ==========
	Inputs:  char *args[]
	Returns: 1 if executed successfully, 0 otherwise

	Handles regular and custom (hardcoded) command cases
	Intended to be called right after command is split by space
	==================================*/

	/*======== void redirect() ==========
	Inputs:  char *args[]
	Returns: void

	helper for execute() when command contains
		redirection characters

	Handles regular command cases involving redirection
	Intended to be called when needed in execute()
	==================================*/

	/*======== void pipe_command() ==========
	Inputs:  int *args[], int option
	Returns: void

	Input Option: 
	      0 if first cmd in args is first cmd of pipe str
	      1 if first cmd in args is in middle of pipe str
	      2 if first cmd in args is last cmd of popup	

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

	prints contents of ~/.custom_shell_history_gt_cv_wx
	       to stdout
	==================================*/

	/*======== void printdir() ==========
	Inputs:  void
	Returns: void

	prints contents of cwd
	       (preceding before shell prompt)
	==================================*/