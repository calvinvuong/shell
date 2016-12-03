#include "util.h"
#include "shell.h"

/*======== int execute() ==========
  Inputs:  char *args[]
  Returns: 1 if executed successfully, 0 otherwise

  char *args[]: array of command line argument strings
	
  Handles regular and custom (hardcoded) command cases
  Intended to be called right after command is split by space
  ==================================*/
int execute(char *args[]) {
  // custom command handling
  if( !strcmp(args[0], "exit") ) { 
    printf(ANSI_COLOR_RED "Exiting shell...\n" ANSI_COLOR_RESET);
    return 1;
    //    exit(0);
  }
  else if ( !strcmp(args[0], "cd") ) {
    chdir(args[1]);
    return 0;
  }
  else if ( !strcmp(args[0], "history") ) {
    show_history();
    return 0;
  }

  // regular command handling
  int f = fork();
  if ( f == 0 )  {
    if ( find_str_in_array(args, "<") != -1 || find_str_in_array(args, ">") != -1 ) {
      redirect(args);
      return 0;
    }
    else if ( find_str_in_array(args, "|") != -1 ) {
      pipe_command(args, 0);
      return 0;
    }
    else
      execvp(args[0], args);
  }
  else
    wait(&f);
  
  return 0;
}

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
void redirect(char *args[]) {
  int output_arrow = find_str_in_array(args, ">");
  int input_arrow = find_str_in_array(args, "<");
  int first_arrow; // arrow in lowest index
  char * output_file_name;
  char * input_file_name;

  if ( input_arrow != -1 ) {
    input_file_name = args[input_arrow + 1];
    int fd_input = open(input_file_name, O_RDONLY);
    if ( fd_input == -1 ) { // file does not exist
      printf("%s: No such file\n", input_file_name);
      return;
    }
    dup2(fd_input, STDIN_FILENO); // redirect stdin to file_name
    close(fd_input);
  }
  else {
    input_file_name = NULL;
    first_arrow = output_arrow;
  }
  
  if ( output_arrow != -1 ) {
    output_file_name = args[output_arrow + 1];
    int fd_output = open(output_file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    dup2(fd_output, STDOUT_FILENO); // redirect stdout to file_name
    close(fd_output);
  }
  else {
    output_file_name = NULL;
    first_arrow = input_arrow;
  }
  
  // both arrows exist; find earliest arrow
  if ( output_arrow > -1 && input_arrow > -1 ) {
    if ( input_arrow < output_arrow )
      first_arrow = input_arrow;
    else
      first_arrow = output_arrow;
  }
  
  args[first_arrow] = 0; // null terminate
  execvp(args[0], args);
}

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
void pipe_command(char *args[], int option) {
  char file_name[] = ".pipe_output";
  char tmp_file[] = ".pipe_output_tmp";
  
  if ( option == 2 ) {
    int f = fork();
    if ( f == 0 ) {
      int fd = open(file_name, O_RDONLY);
      dup2(fd, STDIN_FILENO);
      close(fd);

      execvp(args[0], args);
    }
    else
      wait(&f);

    // remove pipe output file
    unlink(file_name);
  }

  else if ( option == 0 ) {
    int pipe_pos = find_str_in_array(args, "|");
    char ** cmd2 = &(args[pipe_pos+1]);
    args[pipe_pos] = 0;
    
    int f = fork();
    if ( f == 0 ) {
      int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
      dup2(fd, STDOUT_FILENO);
      close(fd);

      execvp(args[0], args);
    }
    else
      wait(&f);
    
    if ( find_str_in_array(cmd2, "|") != -1 ) // more pipes!
      pipe_command(cmd2, 1);
    else // no more pipes
      pipe_command(cmd2, 2);
  }

  // DOES NOT WORK CORRECTLY, but does not break code
  else if ( option == 1 ) {
    int pipe_pos = find_str_in_array(args, "|");
    char ** cmd2 = &(args[pipe_pos+1]);
    args[pipe_pos] = 0;

    int STDIN_FILENO_DUP = dup(STDIN_FILENO);
    int STDOUT_FILENO_DUP = dup(STDOUT_FILENO);
    
    int f = fork();
    if ( f == 0 ) {
      
      int fd1 = open(file_name, O_RDONLY);
      int fd2 = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
      dup2(fd1, STDIN_FILENO);
      dup2(fd2, STDOUT_FILENO);
      close(fd1);
      close(fd2);

      int f2 = fork();
      if ( f2 == 0 )
	execvp(args[0], args);
      else
	wait(&f2);
      rename(tmp_file, file_name);
    }

    if ( find_str_in_array(cmd2, "|") != -1 ) // more pipes!
      pipe_command(cmd2, 1);
    else
      pipe_command(cmd2, 2);

    dup2(STDIN_FILENO_DUP, STDIN_FILENO);
    dup2(STDOUT_FILENO_DUP, STDOUT_FILENO);
    close(STDIN_FILENO_DUP);
    close(STDOUT_FILENO_DUP);
  }
}

/*======== void store_history() ==========
  Inputs:  char *str, int num
  Returns: void

  str: command to be stored
  num: number entry of cmd (should be in chronorder)

  updates ~/.custom_shell_history_gt_cv_wx file
  that stores history of commands
  ==================================*/
void store_history(char *str, int num) {
  char path[1000];
  sprintf(path, "%s/.custom_shell_history_gt_cv_wx", getenv("HOME"));
  int fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);

  // write command #
  char num_str[100];
  sprintf(num_str, "%d\t", num);
  write(fd, num_str, strlen(num_str));

  // write command
  write(fd, str, strlen(str));
  write(fd, "\n", strlen("\n"));

  close(fd);
}

/*======== void show_history() ==========
  Inputs:  void
  Returns: void

  prints contents of ~/.custom_shell_history_gt_cv_wx to stdout
  ==================================*/
void show_history() {
  char path[1000];
  sprintf(path, "%s/.custom_shell_history_gt_cv_wx" , getenv("HOME"));
  int fd = open(path, O_RDONLY);
  // do not need to check if file does not exist b/c history command creates file anyways

  struct stat buf;
  stat(path, &buf);
  int size = buf.st_size;

  char contents[size+1];
  read(fd, contents, size);
  contents[size] = 0; // null terminate
  
  printf(ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,  contents);

}

/*======== void printdir() ==========
  Inputs:  void
  Returns: void

  prints contents of cwd, with the home address abbrievated with ~
  (preceding before shell prompt)
  ==================================*/
void printdir() {
  char dir[1000];  
  getcwd(dir, 1000);

  if ( strstr(dir, getenv("HOME")) == 0 ) // if cwd is above home dir
    printf(ANSI_COLOR_CYAN "%s$ " ANSI_COLOR_RESET, dir);
  else {
    printf(ANSI_COLOR_CYAN "~%s$ " ANSI_COLOR_RESET, strstr(dir, getenv("HOME")) + strlen(getenv("HOME")));
  }
}


int main() {
  int command_num = 0;
  int status = 0;
  
  while ( 1 ) {
    //printf(">>> ");
    printdir();
    char input[1000];
    fgets(input, sizeof(input), stdin);
    *strchr(input, '\n') = 0; //get rid of newline

    // stor4 history
    store_history(input, command_num);
    command_num++;
    
    //array of commands separated by semicolons
    char ** commands = split(input, ";");
    
    int i;
    for (i = 0; commands[i] != NULL; i++) {
      // handle whitespace problems
      char * command_nonsplit_nonWhitespaceBeGoned = whitespaceBeHere( commands[i] );
      char * command_nonsplit = whitespaceBeGone( command_nonsplit_nonWhitespaceBeGoned );
      
      // array of arguments separated by space
      char ** command = split(command_nonsplit, " ");

      status = execute(command);

      // free allocated memory
      free(command_nonsplit_nonWhitespaceBeGoned);
      free(command_nonsplit);
      free(command);
      command_nonsplit_nonWhitespaceBeGoned = NULL;
      command_nonsplit = NULL;
      command = NULL;
      
    }  

    free(commands);
    commands = NULL;
    // if exit command called
    if ( status == 1 )
      exit(0);
  }

  return 0;
}

