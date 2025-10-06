#include "wish.h"

// waits for user input/user to type exit
 
int main(int argc, char *argv[]) {
    // argument validation
    if (argc > 2) {
        printError();
        exit(1);
    }
 
    FILE *input = stdin;
    int interactive = 1;
 
    // batch mode setup
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            printError();
            exit(1);
        }
        interactive = 0;
    }
 
    char *line = NULL;
    size_t len = 0;
 
    while (1) {
        // print prompt only in interactive mode
        if (interactive) {
            printf(PROMPT);
            fflush(stdout);
        }
 
        // read command line
        if (getline(&line, &len, input) == -1) {
            if (feof(input)) {
                free(line);
                if (input != stdin) fclose(input);
                exit(0); // normal exit
            } else {
                printError();
                continue;
            }
        }
 
        // remove leading/trailing spaces
        char *trimmed = trimWs(line);
        if (*trimmed == '\0') // ignore empty lines
            continue;
 
        // split
        char **argvv = splitWhitespace(trimmed);
        if (argvv[0] == NULL)
            continue;
 
        // for now, just print the parsed command
        printf("Command: %s\n", argvv[0]);
        for (int i = 1; argvv[i]; i++) {
            printf("Arg[%d]: %s\n", i, argvv[i]);
        }
 
        // free allocated argument list
        for (int i = 0; argvv[i]; i++) free(argvv[i]);
        free(argvv);
    }
 
    // cleanup (good practice, will probably never run tho)
    free(line);
    if (input != stdin) fclose(input);
    return 0;
}
