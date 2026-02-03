#include <stdio.h>
#include <string.h>

void trim(char* input, int x);

int main() {
    char input[512];
    char letter;
    char* token;
    int i = 0;
    char tempinput[512];
    char* delimOne = {"|"};
    char* delimTwo = {"&"};
    char* delimThree = {">"};
    char* delimFour = {"<"};
    char* delimFive = {";"};
    char tokens[50][50] = {};
    int tokensIdx = 0;
    
    printf("enter:\n");
    fgets(input, 512, stdin);
    
    while((letter = input[i]) != '\0'){
        if(letter == '|'){
            strcpy(tempinput,input);
            token = strtok(tempinput, "|");
            strcat(token, delimOne);
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else if(letter == '&'){
            strcpy(tempinput,input);
            token = strtok(tempinput, "&");
            strcat(token, delimTwo);
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else if(letter == '>'){
            strcpy(tempinput,input);
            token = strtok(tempinput, ">");
            strcat(token, delimThree);
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else if(letter == '<'){
            strcpy(tempinput,input);
            token = strtok(tempinput, "<");
            strcat(token, delimFour);
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else if(letter == ';'){
            strcpy(tempinput,input);
            token = strtok(tempinput, ";");
            strcat(token, delimFive);
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else if(letter == ' '){
            strcpy(tempinput,input);
            token = strtok(tempinput, " ");
            trim(input,i);
            strcpy(tokens[tokensIdx], token);
            tokensIdx++;
            i = 0;
        }
        
        else{
            i = i + 1;
        }
        
        
    }
    strcpy(tokens[tokensIdx], input);
    
    for(int x=0; x<10; x++){
        printf("%s\n",tokens[x]);
    }
    

    return 0;
}

void trim(char* input, int x){
    int j = 0;
    int i = x;
    i++;
    while(input[j++] = input[i++]);
}