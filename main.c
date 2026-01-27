#include <unistd.h> // fork
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "shelllib.h"




int main(int argc, char const *argv[])
{
    /* code */
    int loop = 1;
    char cwd[100];
    char input[100];
    while (strcmp(input, "exit") != 10){
        output(input);
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
    //int test = strcmp(arg, "exit");
    //printf("%d", test);
    // if (strcmp(arg, "exit") == 0){
    //     loop = 0;
    //     printf("YOOOO");
    // }
    return;
}
