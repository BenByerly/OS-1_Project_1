#include "wish.h"

void executeCommand(char **argv) {
    char **pathDirs = getPathDirs();

    if (!pathDirs || !pathDirs[0]) {
        pathError();
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        pathError();
        return;
    }

    if (pid == 0) {
        for (int i = 0; pathDirs[i]; i++) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pathDirs[i], argv[0]);
            if (access(path, X_OK) == 0) {
                execv(path, argv);
                pathError();
                _exit(1);
            }
        }

        // if no path worked command not found error given
        pathError();
        _exit(1);
    } else {
        // parent waits for child to finish
        int status;
        waitpid(pid, &status, 0);
    }
}
