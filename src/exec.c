/*
      Last edited by Levi S. and Benjamin B.     10/6/2025
 
*/


// exec.c
#include "wish.h"

void executeCommand(char **argv) {
    char **pathDirs = getPathDirs();

    if (!pathDirs || !pathDirs[0]) {
        printError();
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        printError();
        return;
    }

    if (pid == 0) {
        for (int i = 0; pathDirs[i]; i++) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pathDirs[i], argv[0]);
            if (access(path, X_OK) == 0) {
                execv(path, argv);
                printError();
                _exit(1);
            }
        }

        // if no path worked command not found error given
        printError();
        _exit(1);
    } else {
        // parent waits for child to finish
        int status;
        waitpid(pid, &status, 0);
    }
}
