#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>

int checkOption(char *optarg, char option, int optdefault);
void checkPID(char *p, int s, int U, int S, int v, int c);
void listProcess(char *pid, int s, int U, int S, int v, int c);
