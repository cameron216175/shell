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


char *cwd; // Define cwd globally, This is so we can access it anywhere in the program.

int main(int argc, char const *argv[])
{
    /* code */
    const char *HOME = getenv("HOME");
    cwd = HOME;
    int loop = 1;
    DIR *dr = opendir(cwd);
    if(dr == NULL){
        printf("Error: %s is not a valid directory! (%s)\n", cwd, strerror(errno));
        exit(1);
    }
    closedir(dr);

    char input[100];
    while (!feof(stdin)){
        printf("[%s] ~", cwd);
        fgets(input,sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0;
        token(input);
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

    //struct dirent *de;

    pid_t p = fork();

    if (p < 0){
        //fork has failed
        printf("fork failed...help!");
    }

    else if (p == 0){
        //child fork
        
        if (execl("/bin/ls", "ls", "-l", "-a", NULL) == -1){
            int n = errno;
            
            perror("Error!!");
            printf("Error number %d\n", n);
        }
    }
    else if (p > 0){
        wait(NULL);
    }
}

void quit(char* arg){
    exit(0);
}
