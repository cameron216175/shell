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
//void getpath(char** arg);

struct command {
    const char *name;
    command_fn fn;
};

struct aliasMap {
    char name[64];
    char command[256];
};

struct command commands[] = {
    {"getpath",getpath},
    {"setpath", setpath},
    {"cd", cd},
    {"exit", quit},
    {"alias", alias},
    {"unalias", unalias}
};


// struct aliasMap aliases[10] = {
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""},
//     {"",""}
// };


// Use for testing full aliases
// struct aliasMap aliases[10] = {
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"},
//     {"bleh","blah"}
// };
