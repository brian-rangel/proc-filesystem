#include "functions.h"

// The main function will parse the user's command-line arguments.
// This program will function similarly to the ps command
int main(int argc, char *argv[]) {
    char *endptr, *pid = "-1"; // Initialized to -1 unless the -p option is provided
    int options, error = 0, s = 0, U = 1, S = 0, v = 0, c = 1;
    long value;

    // Check which options are provided by parsing the command line
    while ((options = getopt(argc, argv, ":p:s::U::S::v::c::")) != -1) {
        switch (options) {
            // -p <pid>: Process information for the specified pid
            case 'p':
                // Check if the provided argument is valid
                value = strtol(optarg, &endptr, 10);
                if (*endptr == '\0') {
                    if (value >= 0) {
                        pid = optarg;
                    } else {
                        printf("TUps: Cannot enter a negative PID for -- 'p'\n");
                        error = 1;
                    }
                } else {
                    printf("TUps: '%s' is not a valid PID argument for -- 'p'\n", optarg);
                    error = 1;
                }
                break;
            // -s: Single-character state information about a process
            case 's':
                s = checkOption(optarg, 's', 0);
                if (s == -1) {
                    error = 1;
                }
                break;
            // -U: User time consumed by a process
            case 'U':
                U = checkOption(optarg, 'U', 1);
                if (U == -1) {
                    error = 1;
                }
                break;
            // -S: System time consumed by a process
            case 'S':
                S = checkOption(optarg, 'S', 0);
                if (S == -1) {
                    error = 1;
                }
                break;
            // -v: Virtual memory used by a process
            case 'v':
                v = checkOption(optarg, 'v', 0);
                if (v == -1) {
                    error = 1;
                }
                break;
            // -c: The command-line that started a process
            case 'c':
                c = checkOption(optarg, 'c', 1);
                if (c == -1) {
                    error = 1;
                }
                break;
            // The user has provided an unknown option
            case '?':
                printf("TUps: Unknown option provided -- '%c'\n", optopt);
                error = 1;
                break;
            // The user has failed to provide a required argument for an option
            case ':':
                printf("TUps: Missing argument for -- '%c'\n", optopt);
                error = 1;
                break;
        }
    }

    // Close the program if errors were found
    if (error) {
        return 1;
    } else {
        // Read from the proc file system
        checkPID(pid, s, U, S, v, c);
    }
    return 0;
}
