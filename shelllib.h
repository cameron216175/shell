typedef void (*command_fn)(char **);
void cd(char** arg);
void output(char* arg);
void token(char* arg);
void execute_command(char** tokens);
void externalCommands(char** arg);
void quit(char** arg);
void getpath(char** arg);
void setpath(char** arg);
void prhist(char** arg);
//void getpath(char** arg);

struct command {
    const char *name;
    command_fn fn;
};

struct command commands[] = {
    {"getpath",getpath},
    {"setpath", setpath},
    {"cd", cd},
    {"exit", quit},
    {"prhist", prhist}
};



