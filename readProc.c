#include "functions.h"

void checkOptions(char *pid, int s, int U, int S, int v, int c) {
    // Check if the -p option was provided
    if (strcmp(pid, "-1") == 0) {
        struct dirent *dirent; // Pointer to struct dirent
        DIR *dir = opendir("/proc"); // Directory stream
        
        // Check if program failed to open the directory
        if ((dir = opendir("/proc")) == NULL){
            printf("TUps: Failed to open '/proc'");
            return;
        } else {
            while ((dirent = readdir(dir)) != NULL) {
                // Check if current directory entry is a process
                if (atoi(dirent->d_name) != 0) {
                    // Create a path leading to the process uid
                    char path[BUFSIZ];
                    strcpy(path, "/proc/");
                    strcat(path, dirent->d_name);
                    strcat(path, "/loginuid");

                    // Only list the process if it belongs to the current user
                    FILE *file = fopen(path, "r");
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
    }
    else {
        listProcess(pid, s, U, S, v, c);
    }
}

void listProcess(char *pid, int s, int U, int S, int v, int c) {
    char *token, *pointer, path[BUFSIZ];
    FILE *file;
    printf("%s: ", pid);

    // Create a path leading to the process stat
    strcpy(path, "/proc/");
    strcat(path, pid);
    strcat(path, "/stat");
    file = fopen(path, "r");

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
        file = fopen(path, "r");

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
        // Create a path leading to the process command line
        strcpy(path, "/proc/");
        strcat(path, pid);
        strcat(path, "/cmdline");
        file = fopen(path, "r");

        // Print the entire process command line character by character
        char currentChar, nextChar = fgetc(file);
        printf("[");
        while (nextChar != EOF) {
            currentChar = nextChar;
            nextChar = fgetc(file);
            // Check if the current character is the '\0' delimeter
            if (currentChar != '\0') {
                printf("%c", currentChar);
            } else {
                if (nextChar != EOF) {
                    printf(" ");
                }
            }
        }
        printf("]");
        fclose(file);
    }
    printf("\n");
}
