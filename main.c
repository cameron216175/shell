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


char *cwd = NULL;
char *path = NULL;

int main(int argc, char const *argv[])
{
    // Program init ---
    const char *HOME = getenv("HOME");
    path = getenv("PATH");

    if (chdir(HOME) != 0 || HOME == NULL) {
        printf("Error: %s (%s)\n", HOME, strerror(errno));
        exit(1);
    }

    cwd = strdup(HOME);

    char input[100];
   // -------

    while (!feof(stdin)){  // Main loop
        printf("[%s] >> ", cwd);
        fgets(input,sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0;
        token(input);
    }
    return 0;
}


void output(char* arg){
    printf("%s \n", arg);
    return;
}

void token(char* arg){
    char *tokens[32] = {};
    char *token = strtok(arg, " \t;|><&");
    int i = 0;
    while (token != NULL){
        tokens[i++] = token;
        token = strtok(NULL, " \t;|><&");
    }

    // for (int i = 0; i < 32; i++){
    //     printf("%s",tokens[i]);
    // }
    execute_command(tokens);

    return;
}


void execute_command(char **tokens){
    
    if (!tokens[0]) return; // Check if tokens exists, if no tockens return.


    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++){  // Loop through all elements in the struct
        if (strcmp(tokens[0], commands[i].name) == 0){ // Check if the first inputed token matches the current loop-struct-element
            commands[i].fn(tokens); // if found, execute the related function pointer, passing in all tokens as arguments
            return; // return after command execusion 
        }
    }

    externalCommands(tokens); // If command was not found in the intenral command struct, attempt to run as an external command.

    //printf("Error: command %s not found!\n", tokens[0]);
}

void cd(char** path){

    if (path[1] == NULL){
        const char *HOME = getenv("HOME");
        if (chdir(HOME) != 0){
            printf("Error: %s (%s)\n", path[1], strerror(errno));
            return;
        }
    }   else if (chdir(path[1]) != 0) {
        printf("Error: %s (%s)\n", path[1], strerror(errno));
        return;
    }

    strcpy(cwd,getcwd(NULL, 0));

    return;
}

void externalCommands(char** args){
    //struct dirent *de;

    pid_t p = fork();

    if (p < 0){
        //fork has failed
        printf("fork failed...help!");
    }

    else if (p == 0){
        //child fork
        
        if (execvp(args[0], args) == -1){
            printf("Error: %s (%s)\n", *args, strerror(errno));
        }
        exit(0);
    }
    else if (p > 0){
        wait(NULL);
    }
}

void getpath(char** arg) {
    if (arg[1]) {
        output("Error: This command does not require arguments"); 
        return;
    }
    output(path);
    return;
}

void setpath(char** arg) {
    if (!arg[1]){
        output("Error: This command requires and arguement"); 
        return;
    } else if (arg[2]){
        output("Error: This command takes only 1 command"); 
        return;
    } 
    strcpy(path, arg[1]);
    return;
}


void quit(char** arg){
    exit(0);
}

void alias(char** args){

    //skip if there are not enough arguments
    if (args[2] == NULL){
        printf("Too few arguments provided\n");
        return;
    }
    
    int aliasPosition = 0;

    //check if an alias with the same name already exists
    for (int i = 0; i < 10; i++){
        if (aliases[i].name){
            if(strcmp(args[1], aliases[i].name) == 0){
                printf("Error: alias already exists\n");
                return;
            }
        }
    }

    while (aliases[aliasPosition].name){
        aliasPosition++;
    }

    //populate alias tuple (alias, command)
    aliases[aliasPosition].name = args[1];

    //combine all remaining args into a single string
    int i = 2;
    char newArgs[100] = {};
 
    while(args[i]){
        strcat(newArgs, args[i]);
        strcat(newArgs, " "); 
        i++;
    }

    //populate alias tuple (alias, command)
    aliases[aliasPosition].command = newArgs;
    

    printf("%s\n%s\n", aliases[0].name, aliases[0].command);

    return;
}