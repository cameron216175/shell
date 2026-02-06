typedef void (*command_fn)(char *);
void output(char* arg[32]);
void token(char* arg);
void execute_command(char **tokens, int length);
void quit(char* arg);



