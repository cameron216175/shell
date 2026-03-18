typedef void (*command_fn)(char **);
void cd(char** arg);
void output(char* arg);
void token(char* arg);
void execute_command(char **tokens);
void externalCommands(char** arg);
void quit(char** arg);
void getpath(char** arg);
void setpath(char** arg);
void alias(char** args);
void unalias(char** arg);
char** toCommand(char* alias);
void saveAliases();
void loadAliases();

struct command {
    const char *name;
    command_fn fn;
};

struct aliasMap {
    char name[64];
    char command[256];
};

struct command commands[] = {
    {"getpath", getpath},
    {"setpath", setpath},
    {"cd", cd},
    {"exit", quit},
    {"alias", alias},
    {"unalias", unalias}
};
