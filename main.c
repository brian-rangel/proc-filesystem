#include "functions.h"

int main(int argc, char *argv[]) {
    int opt;

    // Parse the command line
    while ((opt = getopt(argc, argv, "p::sUSvc")) != -1) {
        switch (opt) {
            case 'p':
                printf("Argument -p was provided\n");
                break;
            case 's':
                printf("Argument -s was provided\n");
                break;
            case 'U':
                printf("Argument -U was provided\n");
                break;
            case 'S':
                printf("Argument -S was provided\n");
                break;
            case 'v':
                printf("Argument -v was provided\n");
                break;
            case 'c':
                printf("Argument -c was provided\n");
                break;
        }
    }

    // Read from the proc file system
    readProc();
    return 0;
}