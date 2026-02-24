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

struct aliasMap aliases[10] = {
    {"", ""}
};

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

    //if its an alias
    for (int i = 0; i < 10; i++){
        if (strcmp(aliases[i].name, tokens[0]) == 0){
            tokens[0] = aliases[i].command;
            execute_command(tokens);
            return;
        }
    }



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
    } else if (chdir(path[1]) != 0) {
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
        if (args[1] == NULL){
            int i = 0;
            while (aliases[i].name[0] != '\0'){
                printf("%d. %s %s\n", (i+1), aliases[i].name, aliases[i].command); // alias with no commands prints out all currently saved aliases
                i++;
            }
            return;
        }
        printf("Too few arguments provided for aliasing\n");
        return;
    }
    
    int aliasPosition = 0;
    
    if(strcmp(aliases[9].name, "") != 0){
        printf("Max number (10) aliases reached\n");
        return;
    }

    //check if an alias with the same name already exists
    for (int i = 0; i < 10; i++){
        if(strcmp(args[1], aliases[i].name) == 0){
            printf("Error: alias already exists\n");
            return;
        }
    }

    //get position
    while (aliases[aliasPosition].name[0] != '\0'){
        aliasPosition++;
    }

    //combine all remaining args into a single string
    int i = 2;
    char newArgs[100] = {};

    while(args[i]){
        strcat(newArgs, args[i]);
        if (!args[i++]){
            strcat(newArgs, " ");
        }
        i++;
    }

    //populate alias tuple (alias, command)
    strcpy(aliases[aliasPosition].name, args[1]);
    strcpy(aliases[aliasPosition].command, args[2]);

    return;
}

void unalias(char** arg){


    int aliasPosition = 0;

    if (arg[1] == NULL){
        printf("No arguments found\n");
        return;
    }

    if (aliases[0].name[0] == '\0'){
        printf("No aliases to unalias\n");
        return;
    }

    //find position
    for (int i = 0; i < 10; i++){
        if (strcmp(aliases[i].name, arg[1]) == 0){
            aliasPosition = i;
        }
    }

    //set name[0] and command[0] = '\0'
    aliases[aliasPosition].name[0] = '\0';
    aliases[aliasPosition].command[0] = '\0';


    //move aliases back to lowest places in array
    for (int i = 1; i < 10; i++){
        if (aliases[i-1].name[0] == '\0' && aliases[i].name[0] != '\0'){ //if there is a value after an empty 
            strcpy(aliases[i-1].name, aliases[i].name); //move them back one place
            strcpy(aliases[i-1].command, aliases[i].command);
            aliases[i].name[0] = '\0'; 
            aliases[i].command[0] = '\0';
            i--; // PROBABLY NOT NECCESSARY - PERHAPS DUPLICATING WORK
        }
    }

    return;
}