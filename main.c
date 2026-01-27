#include <unistd.h> // fork
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <stdio.h>
#include <unistd.h>
#include "shelllib.h"




int main(int argc, char const *argv[])
{
    /* code */
    char cwd[100];
    char input[100];
    while (true){
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("[%s] >> ", cwd);
       fgets(input,sizeof(input),stdin);
    } else {
       perror("getcwd() error");
       return 1;
    }
    }
    return 0;
}

void output(char* arg){
    printf(arg, "\n");
    return;
}
