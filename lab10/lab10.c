/* Author(s): Daniel Shevchyk.
 *
 * This is lab9.c the csc60mshell
 * This program serves as a skeleton for doing labs 9, 10, 11.
 * Student is required to use this program to build a mini shell
 * using the specification as documented in direction.
 * Date: Spring 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXLINE 80
#define MAXARGS 20
#define MAX_PATH_LENGTH 50
#define TRUE 1

/* function prototypes */
int parseline(char *cmdline, char **argv);

//The two function prototypess below will be needed in lab10. 
//Leave them here to be used later.
 void process_input(int argc, char **argv); 
 void handle_redir(int count, char *argv[]); 

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */
int main(void)
{
    char cmdline[MAXLINE];
    char *argv[MAXARGS];
    int argc;
    int status;
    int i;
    pid_t pid;

    /* Loop forever to wait and process commands */
    while (TRUE) {
	/* Print your shell name: csc60mshell (m for mini shell) */
	printf("csc60mshell> ");

	/* Read the command line */
	fgets(cmdline, MAXLINE, stdin);

	/* Call parseline to build argc/argv */
        argc = parseline(cmdline,argv);
        printf("argc= %d \n", argc);

        for(i=0;i<argc;i++)
        {
            printf("argv[%i] = %s \n", i, argv[i]);
        }

	/* If user hits enter key without a command, continue to loop */
	/* again at the beginning */
	/*  Hint: if argc is zero, no command declared */
	/*  Hint: look up for the keyword "continue" in C */
        if(argc ==0)
        {
            printf("no command entered. \n");
            continue;
        }
        
	/* Handle build-in command: exit, pwd, or cd  */
	/* Put the rest of your code here */
        char Commands[argc];
        strcpy(Commands, *argv);
        if(strcmp(Commands, "exit") == 0)
        {
            exit(EXIT_SUCCESS);
 
        }
        else if(strcmp(Commands, "pwd") == 0)
        {
            char path[MAX_PATH_LENGTH];
            getcwd(path, MAX_PATH_LENGTH);
            printf("Current Path: %s \n", path);
            continue;
        }
        else if(strcmp(Commands, "cd")==0)
        {
            char *dir;
            if(argc==1)
            {
                dir = getenv("HOME");
                printf("HOME = %s \n", dir);
            }
            else
                dir = argv[1];
            if(chdir(dir) !=0){   
               perror("error changing directory");
            }
            continue;
       } 
           
 
//.......................IGNORE........................	
//	/* Else, fork off a process */
      else {
	    pid = fork();
          switch(pid)
	    {
	    	case -1:
		    perror("Shell Program fork error");
	            exit(EXIT_FAILURE);
	   	case 0:
		    /* I am child process. I will execute the command, */
		    /* and call: execvp */
		    process_input(argc, argv);
		    break;
	   	default:
		    /* I am parent process */
		    if (wait(&status) == -1)
		    	perror("Parent Process error");
		    else
		    	printf("Child returned status: %d\n",status);
		    break;
	   }	/* end of the switch */
//...end of the IGNORE above.........................
	}	/* end of the if-else-if */
    }		/* end of the while */
} 		/* end of main */

/* ----------------------------------------------------------------- */
/*                  parseline                                        */
/* ----------------------------------------------------------------- */
/* parse input line into argc/argv format */

int parseline(char *cmdline, char **argv)
{
    int count = 0;
    char *separator = " \n\t"; /* Includes space, Enter, Tab */
 
    /* strtok searches for the characters listed in separator */
    argv[count] = strtok(cmdline, separator);

    while ((argv[count] != NULL) && (count+1 < MAXARGS)) 
    	argv[++count] = strtok((char *) 0, separator);
     		
    return count;
}
/* ----------------------------------------------------------------- */
/*                  process_input                                    */
/* ----------------------------------------------------------------- */
void process_input(int argc, char **argv) {                       

    /* Step 1: Call handle_redir to deal with operators:            */
    /* < , or  >, or both                                           */
    handle_redir(argc, argv);    
        
    /* Step 2: perform system call execvp to execute command        */
    /* Hint: Please be sure to review execvp.c sample program       */
    /* if (........ == -1) {                                        */
    /*    fprintf(stderr, "Error on the exec call\n");              */
    /*    _exit(EXIT_FAILURE);                                      */
    /* }                                                            */
    int returned_value;
    returned_value = execvp( argv[0], argv);
    if(returned_value==-1){
        perror("Shell Program");
        _exit(EXIT_FAILURE);
    }
}
/* ----------------------------------------------------------------- */
void handle_redir(int count, char *argv[]){
    int out_redir = 0;
    int in_redir = 0;
    int loop_counter, fd_in, fd_out;

    for(loop_counter = 0; loop_counter < count; loop_counter++){
        if(0 == strcmp(argv[loop_counter], ">")){
            if(out_redir != 0){
                perror("Cannot output to more than one file.");
                _exit(EXIT_FAILURE);
                }
        else if(loop_counter== 0){
            
            perror("No command entered");
            _exit(EXIT_FAILURE);
        }
        out_redir = loop_counter;
        }
        else if(0 == strcmp(argv[loop_counter], "<")){
            if(in_redir !=0){
                perror("Cannot take more than one file.");
                _exit(EXIT_FAILURE);
            }
            else if (loop_counter == 0){
                perror("No Command entered");
                _exit(EXIT_FAILURE);
            }
            in_redir=loop_counter;
        }
        if(out_redir !=0){
            fd_out = open(argv[out_redir+1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            if(fd_out == -1){
                perror("open");
            }
            dup2(fd_out,1);
            if(close(fd_out)==-1)
                perror("clode fd");
            argv[out_redir]= NULL;
        }
        if(in_redir !=0){
            if( 0 == strcmp(argv[in_redir + 1], "EXIT")){
                perror("There is no command.");
                _exit(EXIT_FAILURE);
            }
            fd_in = open(argv[in_redir + 1], O_RDONLY);
                if(fd_in == -1){
                    perror("open");
                }
           dup2(fd_in,0);
                if(close(fd_in) == -1)
                        perror("close fd");
           argv[in_redir] = NULL;
        }
    }
}

/* ----------------------------------------------------------------- */


/* ----------------------------------------------------------------- */
