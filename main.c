#include <unistd.h> // fork
#include <sys/types.h> // pid_t datatype
#include <sys/wait.h> // wait
#include <errno.h> // errno
#include <stdio.h>
#include "shelllib.h"




int main(int argc, char const *argv[])
{
    /* code */
    output("Hello!!!");
    return 0;
}

void output(char* arg){
    printf(arg, "\n");
    return;
}
