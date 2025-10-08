/*
      Last edited by Levi S. and Benjamin B.     10/6/2025
 
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
 
        // split 
	// argvv holds the user's commands 
        char **argvv = splitWhitespace(trimmed);
        if (argvv[0] == NULL)
            continue;
 
        // check builtins
	if (handleBuiltin(argvv)) {
	    for (int i = 0; argvv[i]; i++) {
	        free(argvv[i]);
	    }
	    free(argvv);
	    continue;
	}
	executeCommand(argvv);
 
        // free allocated argument list
        for (int i = 0; argvv[i]; i++) {
	    free(argvv[i]);
	    continue;
	}
        free(argvv);
    }
 
    // cleanup (good practice, will probably never run tho)
    free(line);
    if (input != stdin) {
        fclose(input);
    }
    return 0;
}
