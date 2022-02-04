#include "functions.h"

// Check the current option that is being parsed and return a value
// to decide whether or not it should be displayed in the terminal
int checkOption(char *optarg, char option, int optdefault) {
    // For options that are set to 0 by default
    if (optdefault == 0) {
        if (optarg != NULL && strcmp(optarg, "-") == 0) {
            // Negate the option 
            return 0;
        } else if (optarg == NULL) {
            // Display the option
            return 1;
        }
    } 
    // For options that are set to 1 by default
    else if (optdefault == 1) {
        if (optarg != NULL && strcmp(optarg, "-") == 0) {
            // Negate the option 
            return 1;
        } else if (optarg == NULL) {
            // Don't display the option 
            return 0;
        }
    }
    // Invalid argument for an option
    printf("TUps: '%s' is not a valid argument for -- '%c'\n", optarg, option);
    return -1;
}
