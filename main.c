#include <unistd.h> // fork
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "shelllib.h"




int main(int argc, char const *argv[])
{
    /* code */
    //int loop = 1;
    char cwd[100];
    char input[256];
    while (strcmp(input, "exit") != 10){
        output(input);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
           printf("[%s] >> ", cwd);
           fgets(input,sizeof(input),stdin);
        } else {
           perror("getcwd() error");
           return 1;
        }
        char* tokens = malloc(sizeof(tokens)*256);
        char* token = strtok(input, " \t|>,<&;");
        while (token != NULL){
            strcat(tokens, token);
            token = strtok(NULL, " \t|>,<&;");
        }


        free(tokens);
    }
    return 0;
}

void output(char* arg){
    printf("%s\n", arg);
    //int test = strcmp(arg, "exit");
    //printf("%d", test);
    // if (strcmp(arg, "exit") == 0){
    //     loop = 0;
    //     printf("YOOOO");
    // }
    return;
}
