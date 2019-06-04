#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
/*every process have "num" that 
is less in 1 from his so he can send signal to the next process
*/
int num;
//the pid of the five process
int pids[5];


/*
function that print by the fornat and send signal to 
the next process with "num"
*/
void sigCather(int signo){
    //decler on the signal
    signal(SIGTERM,sigCather);
    printf("PID %d cueat one\n",getpid());
    //the first process dont send signal to enyone
    if(num>-1)
    kill(pids[num],SIGTERM);
}

int main(){
    //pid of the process
    int pid;
    //pid of the child pid
    int cpid;
    //decler on the signal
    signal(SIGTERM,sigCather);

    for(int i=0;i<5;i++){
        //forking five times
         pid=fork();
         // every son :
         if(pid==0){
             //print by the format
            printf("PID %d ready\n",getpid());
            //every process get num that is the index int the pid array of the previos process
            num=i-1;
            //wait for a signal
            pause();
            //killing himself = zombi
            exit(0);
         }
         else{// the father:
         // put the pids in the pids array 
             pids[i]=pid;
             // wait a little before forking -for the order of the process
             sleep(0.1);
         }
    }
    //wait for all the process
    sleep(1);
    // send signal for the last process
    kill(pids[4],SIGTERM);
    //wait for all the process to send  themself a signals
    sleep(1);

    //  killing all the children
    //acording to : https://www.geeksforgeeks.org/wait-system-call-c/
    for(int i=0;i<5;i++){
       //returns a process ID of dead children.
        cpid=wait(NULL);
        printf("Process %d is dead\n",cpid);
        //kill that children
        kill(cpid,SIGKILL);
    }
}