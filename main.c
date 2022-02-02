#include "functions.h"

int main(int argc, char *argv[]) {
    char *pid = "-1"; // Initialized to -1 unless the -p option is provided
    int options, s = 0, U = 1, S = 0, v = 0, c = 1;

    // Check which options are provided by parsing the command line
    while ((options = getopt(argc, argv, "p:sUSvc")) != -1) {
        switch (options) {
            // -p <pid>: Process information for the specified pid
            case 'p':
                pid = optarg;
                break;
            // -s: Single-character state information about a process
            case 's':
                s = 1;
                break;
            // -U: User time consumed by a process
            case 'U':
                U = 0;
                break;
            // -S: System time consumed by a process
            case 'S':
                S = 1;
                break;
            // -v: Virtual memory used by a process
            case 'v':
                v = 1;
                break;
            // -c: The command-line that started a process
            case 'c':
                c = 0;
                break;
        }
    }

    // Read from the proc file system
    checkOptions(pid, s, U, S, v, c);
    return 0;
}
