typedef void (*command_fn)(char **);
void cd(char** arg);
void output(char* arg[32]);
void token(char* arg);
void execute_command(char **tokens);
void ls(char** arg);
void quit(char** arg);
//void getpath(char** arg);

struct command {
    const char *name;
    command_fn fn;
};

struct command commands[] = {
    //{"getpath",getpath},
    {"cd", cd},
    {"exit", quit}
};



