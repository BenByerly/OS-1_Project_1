/*
      Last edited by Gage J.     10/12/2025
 
*/


// main.c
#include "wish.h"

// waits for user input/user to type exit


int main(int argc, char *argv[]) {
    // argument validation
    if (argc > 2) {
        printError();
        exit(1);
    }

    // initiliazes /bin as default path
    initPath();
 
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

        // Split input by '&' for parallel commands
        char *line_copy = strdup(trimmed);
        if (!line_copy) {
            printError();
            continue;
        }
        char **commands = NULL;
        // splitByAmpersand may not be declared in wish.h, so declare here
        extern char **splitByAmpersand(char *line);
        commands = splitByAmpersand(line_copy);
        int parallel_count = 0;
        for (int i = 0; commands[i]; i++) parallel_count++;

        // If only one command, handle as before
        if (parallel_count == 1) {
            char **argvv = splitWhitespace(commands[0]);
            if (argvv[0] == NULL) {
                free(commands[0]);
                free(commands);
                free(line_copy);
                continue;
            }
            if (handleBuiltin(argvv)) {
                for (int i = 0; argvv[i]; i++) free(argvv[i]);
                free(argvv);
                free(commands[0]);
                free(commands);
                free(line_copy);
                continue;
            }
            executeCommand(argvv);
            for (int i = 0; argvv[i]; i++) free(argvv[i]);
            free(argvv);
        } else {
            // For multiple commands, check builtins and execute in parallel
            int has_builtin = 0;
            for (int i = 0; commands[i]; i++) {
                char **argvv = splitWhitespace(commands[i]);
                if (argvv[0] && handleBuiltin(argvv)) {
                    has_builtin = 1;
                }
                for (int j = 0; argvv[j]; j++) free(argvv[j]);
                free(argvv);
            }
            if (!has_builtin) {
                // Only execute in parallel if no builtins present
                extern void executeParallelCommands(char **commands);
                executeParallelCommands(commands);
            }
        }
        // Free commands array
        for (int i = 0; commands[i]; i++) free(commands[i]);
        free(commands);
        free(line_copy);
    }
 
    // cleanup (good practice, will probably never run tho)
    free(line);
    if (input != stdin) {
        fclose(input);
    }
    return 0;
}
