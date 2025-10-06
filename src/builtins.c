#include "wish.h"

static char **pathDirs = NULL;

// initialize the path list with a default directory
void initPath() {
    pathDirs = malloc(2 * sizeof(char *));
    pathDirs[0] = strdup("/bin");
    pathDirs[1] = NULL;
}

// free up memory used by path array
void freePath() {
    if (!pathDirs) return;
    for (int i = 0; pathDirs[i]; i++) free(pathDirs[i]);
    free(pathDirs);
    pathDirs = NULL;
}

// return pointer to current list
char **getPathDirs() {
    return pathDirs;
}

// handle the built in commands
int handleBuiltin(char **argv) {
    if (strcmp(argv[0], "exit") == 0) {
        if (argv[1] != NULL) {
            printError();
            return 1;
        }
        freePath();
        exit(0);
    }

    if (strcmp(argv[0], "cd") == 0) {
        if (!argv[1] || argv[2]) {
            printError();
            return 1;
        }
        if (chdir(argv[1]) != 0) {
            printError();
        }
        return 1;
    }

    if (strcmp(argv[0], "path") == 0) {
        freePath();
        int count = 0;
        while (argv[count + 1]) count++;

        pathDirs = malloc((count + 1) * sizeof(char *));
        for (int i = 0; i < count; i++) {
            pathDirs[i] = strdup(argv[i + 1]);
        }
        pathDirs[count] = NULL;
        return 1;
    }


    return 0;
}
