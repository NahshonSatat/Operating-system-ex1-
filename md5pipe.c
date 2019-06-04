#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "md5.h"

int checkSize;

// the signal handler function - chack if the string is not 32
void sig_handler(int signo)
{
  if (signo == SIGINT){
      if(checkSize != 32){
           printf("the size is not 32!!!\n");
               exit(1);
      }
    }
  }

     
int main()
{
        int pipe1[2]; 
        int pipe2[2]; 

        char usr_str[32];
        int pid;
      // if one of the pipes didnt work
        if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
        {
                printf("Pipe failed.");
                return 1;
        }
       // forking 
        pid = fork() ;
        //if the fork didnt work
        if (pid  < 0) 
        {
                printf("Fork failed");
                return 1;
        }
         //in the Parent 
        else if (pid > 0)
        {
                //get the string from the user
                scanf("%s", usr_str); 
                printf("plain text: %s\n",usr_str);
                char child_str[32];
                //only Writes to the first pipe
                close(pipe1[0]);  
                write(pipe1[1], usr_str, strlen(usr_str) + 1); 
                close(pipe1[1]);  

                //Waits for the child to send a string.
                sleep(0.1);

                //only Reads to the second pipe
                close(pipe2[1]); 
                read(pipe2[0], child_str, 32); 
                close(pipe2[0]);

                //convert to string
                std::string input(child_str);
                input[32] = '\0';
                //what is the size of the string
                checkSize = strlen(input.c_str());
                if (signal(SIGINT, sig_handler) == SIG_ERR)
                printf("\ncan't catch SIGINT\n");
                //sending the signal with the function "sig_handler"
                //if its not 32 the process will die
                kill(0,SIGINT); 
                // if its 32 print and kill the son 
                printf("encrypted by process %d : %s\n",pid, input.c_str());
                kill(pid, SIGKILL);
        }
         // the Child
        else 
        {
                char child_str[20];

                 //only Reads to the first pipe
                close(pipe1[1]); 
                read(pipe1[0], child_str, 20); 
                close(pipe1[0]);

                 //convert the string with md5 lib
                std::string md5_str = md5(child_str);
                 

                //only Writes to the second pipe
                close(pipe2[0]);
                const char *output= md5_str.c_str();
                write(pipe2[1], output, 32);
                close(pipe2[1]);
        }
}
