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

void readHistory(); // Foward declare to keep compiler happee


//void writeHistory();
char *cwd = NULL;
char history [19][512];
char* newpath;
FILE *fptr;


int main(int argc, char const *argv[])
{
    
    // Program init ---
    const char *HOME = getenv("HOME");

    if (chdir(HOME) != 0 || HOME == NULL) {
        printf("Error: %s (%s)\n", HOME, strerror(errno));
        exit(1);
    }

    cwd = strdup(HOME);
    readHistory();

    char input[512];

    while (!feof(stdin)){  // Main loop
        printf("[%s] >> ", cwd);
        fgets(input,sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0;
        addHistory(input);
        token(input);
    }
    //Save aliases to history
    
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

void showHistory(char** args) {
    int i = 0;
    int j = 0;
    while (history[i][0] != '\0') {
        printf("%i: ", i+1);
        while (history[i][j] != '\0')
        {
            printf("%c", history[i][j]);
            j++;
        }
        j=0;
        printf("\n");
        i++;
    }
}
void readHistory(){
    char line[512];
    int count = 0;

    fptr = fopen(".hist_list", "r");
    if (fptr) {
        while (count < 20 && fgets(line, sizeof(line), fptr)) {
            line[strcspn(line, "\n")] = 0;
            strcpy(history[count++], line);
        }
        fclose(fptr);
    }

}
void writeHistory() {

    //setpath("HOME"); <- setpath is broken currenty
    // Error: This command takes only 1 command 
    
    const char *HOME = getenv("HOME");
    if (chdir(HOME) != 0 || HOME == NULL) {
        printf("Error: %s (%s)\n", HOME, strerror(errno));
        exit(1);
    }
    cwd = strdup(HOME);


    char tempArr[20][512] = {0};

    fptr = fopen(".hist_list", "w");
    if (!fptr) return;

    int written = 0;
    for (int i = 0; i < 20 && history[i][0] != '\0'; i++) {
        fprintf(fptr, "%s\n", history[i]);
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

    

    //if its an alias
    for (int i = 0; i < 10; i++){
        if (strcmp(aliases[i].name, tokens[0]) == 0){
            char *aliasTokens[32] = {};
            char aliasCommand[256];
            strcpy(aliasCommand, aliases[i].command);

            //tokenise 
            char *aliasToken = strtok(aliasCommand, " ");
            int j = 0;

            while(aliasToken != NULL){
                aliasTokens[j++] = aliasToken;
                aliasToken = strtok(NULL, " ");
            }
            
            //add command with params to new tokens
            char *newTokens[32] = {};
            int x = 0;
            for(int k = j; k != 0; k--){
                newTokens[x] = aliasTokens[x];
                x++;
            }

            //add tokens to new tokens
            int p = 1;
            while(tokens[p] != NULL){
                newTokens[x] = tokens[p];
                x++;
                p++;
            }

            execute_command(newTokens);
            
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
    free(newpath);
    saveAliases();
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
            if(aliases[0].name[0] == '\0'){
                printf("No aliases set yet\n");
                return;
            }
            
            return;
        }
        printf("Too few arguments provided for aliasing\n");
        return;
    }
    
    int aliasPosition = 0;
    int overriding = 0; //if overriding alias 
    
    
    //check if we are overidding
    for (int i = 0; i < 10; i++){
        if(strcmp(args[1], aliases[i].name) == 0){
            overriding = 1;
        }
    }

    // Check if list is full and we are not overdding
    if(overriding == 0 && strcmp(aliases[9].name, "") != 0){
        printf("Max number (10) aliases reached\n");
        return;
    }


    //get position
    while (aliases[aliasPosition].name[0] != '\0' && strcmp(aliases[aliasPosition].name, args[1]) != 0){
        aliasPosition++;
    }

    //combine all remaining args into a single string
    int i = 2;
    char newArgs[100] = {};

    while(args[i]){
        strcat(newArgs, args[i]);
        strcat(newArgs, " ");
        i++;
    }

    int length = strlen(newArgs) -1;
    newArgs[length] = '\0';

    //populate alias tuple (alias, command)
    strcpy(aliases[aliasPosition].name, args[1]);
    strcpy(aliases[aliasPosition].command, newArgs);

    //check if we are overriding
    if(overriding == 1){
        printf("Alias has been overridden\n");
    }

    return;
}

void unalias(char** arg){


    int aliasPosition = 0;
    int aliasFound = 0;


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
            aliasFound = 1;
        }
    }

    if(aliasFound == 0){
        printf("Alias not found\n");
        return;
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
        }
    }

    return;
}

void saveAliases(){

    FILE* fptr;

    fptr = fopen(".aliases", "w");

    if (fptr == NULL) {
        printf("Error when saving aliases");
    } else {
        int i = 0;
        while(aliases[i].name[0] != '\0'){

            fprintf(fptr, "%s %s\n", aliases[i].name,aliases[i].command);

            i++;
        }
    }

    fclose(fptr);
}

void loadAliases(){
    FILE* fptr;

    fptr = fopen(".aliases", "r");

    if(fptr == NULL){
        fptr = fopen(".aliases", "w");
        loadAliases();
    }
    else{
        char loaded_alias[512];
        int index = 0;

        while (fgets(loaded_alias, sizeof(loaded_alias), fptr)) {

            loaded_alias[strcspn(loaded_alias, "\n")] = 0;

            char *space = strchr(loaded_alias, ' ');
            if (!space) continue;

            *space = '\0';

            strcpy(aliases[index].name, loaded_alias);
            strcpy(aliases[index].command, space + 1);

            index++;
        }

    };

    fclose(fptr);
}