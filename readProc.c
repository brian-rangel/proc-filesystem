#include "functions.h"

void readProc() {
    struct dirent *dirent; // Pointer to struct dirent
    DIR *dir = opendir("/proc"); // Directory stream
    
    // Check if program failed to open the directory
    if ((dir = opendir("/proc")) == NULL){
        printf("TUps: Failed to open '/proc'");
        return;
    }
    
    while ((dirent = readdir(dir)) != NULL) {
        // Check if current directory entry is a process
        if (atoi(dirent->d_name) != 0){
            char *token, *pointer, path[BUFSIZ];

            // Create a path leading to the process uid
            strcpy(path, "/proc/");
            strcat(path, dirent->d_name);
            strcat(path, "/loginuid");

            // Only list the process if it belongs to the user
            FILE *file = fopen(path, "r");
            char uid[BUFSIZ];
            fgets(uid, BUFSIZ, file);
            if (getuid() != atoi(uid)) {
                continue;
            } else {
                printf("%s: ", dirent->d_name);
            }
            fclose(file);
            
            // Create a path leading to the process stat
            strcpy(path, "/proc/");
            strcat(path, dirent->d_name);
            strcat(path, "/stat");
            file = fopen(path, "r");
            
            // Read the process stat
            char stat[BUFSIZ];
            pointer = stat;

            while (fgets(stat, sizeof(stat), file)) {
                int i = 1;
                // The stat string is parsed into a sequence of tokens
                while ((token = strtok_r(pointer, " ", &pointer))) {
                    // Content inside stat: (3)state, (14)utime, (15)stime
                    switch (i) {
                        case 3:
                            printf("%s ", token);
                            break;
                        case 14:
                            printf("utime=%s ", token);
                            break;
                        case 15:
                            printf("stime=%s ", token);
                            break;
                    }
                    i++;
                }
            }
            fclose(file);

            // Create a path leading to the process memory status
            strcpy(path, "/proc/");
            strcat(path, dirent->d_name);
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

            // Create a path leading to the process command line
            strcpy(path, "/proc/");
            strcat(path, dirent->d_name);
            strcat(path, "/cmdline");
            file = fopen(path, "r");

            // Print the entire process command line
            printf("[");
            char currentChar, nextChar = fgetc(file);
            while (nextChar != EOF) {
                currentChar = nextChar;
                nextChar = fgetc(file);
                // Ignore the '\0' delimeter
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
            printf("\n");
        }
    }
    closedir(dir);
}