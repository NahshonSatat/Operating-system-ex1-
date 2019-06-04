#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("received SIGINT\n");
}

int main(void)
{
  if (signal(SIGINT, sig_handler) == SIG_ERR)
  printf("\ncan't catch SIGINT\n");
  int pid =fork();
  if(pid>0)
  kill(pid,SIGINT);
  // A long long wait so that we can easily issue a signal to this process
  //while(1) 
   // sleep(1);
  return 0;
}