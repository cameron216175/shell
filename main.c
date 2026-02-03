#include <unistd.h> // fork
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "shelllib.h"


int main(int argc, char const *argv[])
{
    /* code */
    int loop = 1;
    char cwd[100];
    char input[100];
    while (!feof(stdin)){
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("[%s] >> ", cwd);
            fgets(input,sizeof(input),stdin);
            input[strcspn(input, "\n")] = 0;
            token(input);
        } else {
            perror("getcwd() error");
            return 1;
        }
    }
    return 0;
}

void output(char* arg[32]){
    for (int i = 0; arg[i] != NULL; i++){
        printf("%s      ",arg[i]);
    }
    printf("\n");
    return;
}

void token(char* arg){
    char *tokens[32] = {};

    char *token = strtok(arg, " ");
    int i = 0;
    while (token != NULL){
        tokens[i++] = token;
        token = strtok(NULL, " ");
    }

    execute_command(tokens);

    return;
}


void execute_command(char **tokens){
    if (!tokens[0]) return;

    
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++){
        if (strcmp(tokens[0], commands[i].name) == 0){
            commands[i].fn(tokens[1]);
            return;
        }
    }

    printf("Error: command %s not found!\n", tokens[0]);
}

void cd(char* arg){
    printf("CD!! \n");
    return;
}

void ls(char* arg){
    struct dirent *de;

    if(arg == NULL) arg = ".";

    DIR *dr = opendir(arg);

    if(dr == NULL){
        printf("Error: %s is not a valid directory! (%s)\n", arg, strerror(errno));
        return;
    }

    while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);

    closedir(dr);
    return;
}

void quit(char* arg){
    exit(0);
}