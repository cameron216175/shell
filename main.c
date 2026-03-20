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



void writeHistory();
char *cwd = NULL;
char history [19][512];
char* newpath;

int main(int argc, char const *argv[])
{
    
    // Program init ---
    const char *HOME = getenv("HOME");

    if (chdir(HOME) != 0 || HOME == NULL) {
        printf("Error: %s (%s)\n", HOME, strerror(errno));
        exit(1);
    }
    cwd = strdup(HOME);

    char input[512];

    while (!feof(stdin)){  // Main loop
        printf("[%s] >> ", cwd);
        fgets(input,sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0;
        addHistory(input);
        token(input);
    }
    return 0;
}

void addHistory(char input[512]) {
    if (input[0] == '!') return;

    char tempHistory[19][512];
    
    int i = 0;

    while (i < 18 && history[i][0] != '\0') {
        memcpy(tempHistory[i+1], history[i], 512);
        i++;
    }

    memcpy(tempHistory[0], input, 512);

    for (int j = 0; j <= i; ++j)
        memcpy(history[j], tempHistory[j], 512);


}

void runHistory(char arg1, char arg2) {
    int pos;
    if (arg2 == '\0') {
        pos = (arg1-'0')-1;
    } else {
        pos = ((arg1-'0')*10)+(arg2-'0')-1;
    }
    token(history[pos]);
}

void writeHistory() {
    FILE *fptr;
    char tempArr[20][512] = {0};
    char line[512];
    int count = 0;

    fptr = fopen(".hist_list", "r");
    if (fptr) {
        while (count < 20 && fgets(line, sizeof(line), fptr)) {
            line[strcspn(line, "\n")] = 0;
            strcpy(tempArr[count++], line);
        }
        fclose(fptr);
    }

    fptr = fopen(".hist_list", "w");
    if (!fptr) return;

    int written = 0;
    for (int i = 0; i < 20 && history[i][0] != '\0' && written < 20; i++) {
        fprintf(fptr, "%s\n", history[i]);
        written++;
    }

    for (int i = 0; i < count && written < 20; i++) {
        fprintf(fptr, "%s\n", tempArr[i]);
        written++;
    }

    fclose(fptr);
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

    // if (strchr(tokens[0], '!') == '!') {runHistory(strchr(tokens[0], 1));}
    if (tokens[0][0] == '!') {
        runHistory(tokens[0][1], tokens[0][2]); 
        return;
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
    output(getenv("PATH"));
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
    size_t len = strlen("PATH=") + strlen(arg[1]) + 1;
    free(newpath);
    char* newpath = malloc(sizeof(len));
    strcpy(newpath, "PATH=");             /* copy base */
    strcat(newpath, arg[1]);
    putenv(newpath);
    return;
}


void quit(char** arg){
    writeHistory();
    exit(0);
    free(newpath);

}