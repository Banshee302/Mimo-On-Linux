#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "libroot/src/C++/headers/libroot.h"

int userServiceStopped = 0;

void start_services() {
    const char* home = getenv("HOME");
    if (!home) {
        printf("[services] ERROR: Could not determine home directory.\n");
        return;
    }

    char path[512];
    snprintf(path, sizeof(path), "%s/MimoOnLinux/src/C/Services", home);

    DIR* dir = opendir(path);
    if (!dir) {
        printf("[services] No Services directory found.\n");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_REG) continue;

        if (strcmp(entry->d_name, "UserService") == 0 && userServiceStopped) continue;

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (access(fullpath, X_OK) == 0) {
            printf("[services] Launching: %s\n", entry->d_name);
            char command[1050];
            snprintf(command, sizeof(command), "%s", fullpath);
            system(command);
        }
    }

    closedir(dir);
}

int main() {
    char command[100];

    printf("Mimo on Linux [Mimo Subsystem on Linux]\n");
    printf("Type 'exit' to quit.\n");

    start_services();  // 🔧 Launch services at startup

    while (1) {
        printf(">> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            printf("Exiting Mimo Container..\n");
            break;
        } else if (strcmp(command, "version") == 0) {
            printf("Mimo Container Version CLI v1.0\n");
        } else if (strcmp(command, "arch") == 0) {
            printf("Current architecture: ARM\n");
        } else if (strcmp(command, "security") == 0) {
            printf("libroot v1.0, check Docs for more info.\n");
        } else if (strcmp(command, "stop UserService") == 0) {
            userServiceStopped = 1;
            printf("[UserService] Will not restart for this session.\n");
        } else if (strcmp(command, "libroot") == 0) {
            printf("[libroot] ERROR: No application name provided.\n");
        } else if (strncmp(command, "libroot ", 8) == 0) {
            char* arg = command + 8;

            if (strlen(arg) == 0) {
                printf("[libroot] ERROR: No application name provided.\n");
            } else if (strcmp(arg, "mkpasswd") == 0) {
                init_libroot();
            } else {
                int result = run_as_root(arg);
                if (result != 0) {
                    printf("[libroot] Application failed with code %d\n", result);
                }
            }
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}

