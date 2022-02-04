#include "functions.h"

// Opens the /proc directory and checks the PID for either all
// processes that belong to the user or for a specified process.
// Depends on whether or not the -p <PID> option was provided
void checkPID(char *pid, int s, int U, int S, int v, int c) {
    if (strcmp(pid, "-1") == 0) {
        struct dirent *dirent; // Pointer to struct dirent
        DIR *dir = opendir("/proc"); // Directory stream
        
        // Check if program failed to open the directory
        if ((dir = opendir("/proc")) == NULL) {
            printf("TUps: Failed to open '/proc'");
            return;
        } else {
            // Since no PID was specified, check all directories in the proc filesystem
            while ((dirent = readdir(dir)) != NULL) {
                // Check if current directory entry is a process
                char *endptr;
                strtol(dirent->d_name, &endptr, 10);
                if (*endptr == '\0') {
                    // Create a path leading to the process uid
                    char path[BUFSIZ];
                    strcpy(path, "/proc/");
                    strcat(path, dirent->d_name);
                    strcat(path, "/loginuid");

                    // Open the file
                    FILE *file = fopen(path, "r");
                    if (!file) {
                        printf("TUps: Could not read process login UID in \"%s\"\n", path);
                        return;
                    }

                    // Only list the process if it belongs to the current user
                    char uid[BUFSIZ];
                    fgets(uid, BUFSIZ, file);
                    if (getuid() != atoi(uid)) {
                        continue;
                    } else {
                        listProcess(dirent->d_name, s, U, S, v, c);
                    }
                    fclose(file);
                }
            }
        }
        closedir(dir);
    } else {
        // Check if the specified process exists
        char path[BUFSIZ];
        strcpy(path, "/proc/");
        strcat(path, pid);
        DIR *dir = opendir(path);
        if (!dir) {
            printf("TUps: The process with PID '%s' does not exist\n", pid);
            return;
        }
        closedir(dir);

        // Only list the specified process
        listProcess(pid, s, U, S, v, c);
    }
}

// Display the information of a specified process such as PID, state, user time,
// system time, virtual memory size, and process command-line. The information 
// displayed will vary based on the arguments parsed from the command-line
void listProcess(char *pid, int s, int U, int S, int v, int c) {
    char *token, *pointer, path[BUFSIZ];
    FILE *file;
    printf("%s: ", pid);

    // Create a path leading to the process stat
    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/stat");

    // Open the file
    file = fopen(path, "r");
    if (!file) {
        printf("TUps: Could not read process status in \"%s\"\n", path);
        return;
    }

    // Read the contents inside the process stat
    char stat[BUFSIZ];
    pointer = stat;
    while (fgets(stat, sizeof(stat), file)) {
        int i = 1;
        // The stat string is parsed into a sequence of tokens
        while ((token = strtok_r(pointer, " ", &pointer))) {
            switch (i) {
                case 3:
                    // Check if the -s option was provided
                    if (s == 1) {
                        printf("%s ", token);
                    }
                    break;
                case 14:
                    // Check if the -U option was provided
                    if (U == 1) {
                        printf("utime=%s ", token);
                    }
                    break;
                case 15:
                    // Check if the -S option was provided
                    if (S == 1) {
                        printf("stime=%s ", token);
                    }
                    break;
            }
            i++;
        }
    }
    fclose(file);

    // Check if the -v option was provided
    if (v == 1) {
        // Create a path leading to the process memory status
        strcpy(path, "/proc/");
        strcat(path, pid);
        strcat(path, "/statm");

        // Open the file
        file = fopen(path, "r");
        if (!file) {
            printf("\nTUps: Could not read process memory status in \"%s\"\n", path);
            return;
        }

        // Read the process memory status
        char statm[BUFSIZ];
        pointer = statm;
        while (fgets(statm, sizeof(statm), file)) {
            int i = 0;
            // The statm string is parsed into a sequence of tokens
            while ((token = strtok_r(pointer, " ", &pointer))) {
                // The 0th entry is the total program size
                if (i == 0) {
                    printf("vmsize=%s ", token);
                }
                i++;
            }
        }
        fclose(file);
    }

    // Check if the -c option was provided
    if (c == 1) {
        // Create a path leading to the process command-line
        strcpy(path, "/proc/");
        strcat(path, pid);
        strcat(path, "/cmdline");

        // Open the file
        file = fopen(path, "r");
        if (!file) {
            printf("\nTUps: Could not read process command-line in \"%s\"\n", path);
            return;
        }

        // Print the entire process command line character by character
        char currentchar, nextchar = fgetc(file);
        printf("[");
        while (nextchar != EOF) {
            currentchar = nextchar;
            nextchar = fgetc(file);
            // Check if the current character is the '\0' delimeter
            if (currentchar != '\0') {
                printf("%c", currentchar);
            } else {
                if (nextchar != EOF) {
                    printf(" ");
                }
            }
        }
        printf("]");
        fclose(file);
    }
    printf("\n");
}
