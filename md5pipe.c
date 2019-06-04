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

// void sig_handler(int signo)
// {
//     if (signo == SIGUSR1)
//         printf("received SIGUSR1\n");
//     else if (signo == SIGKILL)
//         printf("received SIGKILL\n");
//     else if (signo == SIGSTOP)
//         printf("received SIGSTOP\n");
// }
void sig_handler(int signo)
{
    //printf("%d\n",checkSize);
  if (signo == SIGINT){
      if(checkSize != 32){
           printf("the size is not 32!!!\n");
               exit(1);
      }
    }
  }

     

  //if(checkSize == 32)&&(checkSize == 32)
    

 //  printf("no\n");


// void checkMD5(int s)
// {
//         if (checkSize == 32)
//         {
//                 printf("yes");
//         }
//         else
//                 exit(1);
// }

int main()
{
        int pipe1[2]; //Used to store two ends of first pipe.
        int pipe2[2]; //Used to store two ends of second pipe.

        char usr_str[32];
        int pid;

        if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
        {
                printf("Pipe failed.");
                return 1;
        }
       
        pid = fork() ;
        if (pid  < 0) //Failed.
        {
                printf("Fork failed");
                return 1;
        }

        else if (pid > 0) //Parent process:
        {
                //printf("plain text : %s\n", usr_str);
                scanf("%s", usr_str); //Scans the string from the user.
                //printf("plain text: %s\n", usr_str);
                char child_str[32];

                close(pipe1[0]);                               //Closes reading of the first pipe.
                write(pipe1[1], usr_str, strlen(usr_str) + 1); //Writes input string.
                close(pipe1[1]);                               //Closes writing of the first pipe.

                wait(NULL); //Waits for the child to send a string.
                close(pipe2[1]); //Closes writing of the second pipe.

                read(pipe2[0], child_str, 32); //Reads string from the child.
                std::string input(child_str);
                input[32] = '\0';
                //printf("%s\n", input.c_str());
                checkSize = strlen(input.c_str());//.size()-6;//strlen(child_str);
                close(pipe2[0]); //Closes reading of the second pipe.
                if (signal(SIGINT, sig_handler) == SIG_ERR)
                printf("\ncan't catch SIGINT\n");
                
                kill(0,SIGINT);
                
                printf("encrypted by process %d : %s\n",pid, input.c_str());

                //printf("before kill\n");
                kill(pid, SIGKILL);
                //printf("after kill.\n");
        }

        else //Child process:
        {
                char child_str[20];

                close(pipe1[1]); //Closes writing end of the first pipe.

                read(pipe1[0], child_str, 20); //Reads a string using first pipe.
                std::string md5_str = md5(child_str);

                close(pipe1[0]);
                close(pipe2[0]);
                const char *output= md5_str.c_str();
        
                write(pipe2[1], output, 32);
                close(pipe2[1]);

                //checkSize = md5_str.size();
                //checkSize = 32;
                // printf("checkSize\n");
                // signal(SIGINT, checkMD5);
                // kill(pid, SIGINT);
        }
}