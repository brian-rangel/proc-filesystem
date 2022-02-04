# Proc Filesystem
The goal of this project is to create a program that simulates the ps command on Linux. The ps command on Linux is a useful utility for viewing data related with the processes on a system, such as a process PID and some other information depending on the options the user inputs. It reads the process information from the virtual files in /proc file-system.

## Project Design
The Makefile builds the program by simply typing the "make" command through the command-line. There is also a "clean" command that removes all .o and executable files.

After building the program, you can list all the processes currently running by simply typing the following command:
````
./TUps
```` 
You can also enter in the following options in order to hide or display certain information about a process:
| Options | Description |
|-|-|
-p \<PID\> | Display process information only for the process who has the specified PID. If this option is not present, then information is displayed for all processes that belong to the current user.
-s | Display the single-character state information about a process.
-U | DO NOT Display the amount of user time consumed by a process. If this option is not present, then user time information will be displayed.
-S | Display the amount of system time consumed so far by a process.
-v | Display the amount of virtual memory currently used by a process.
-c | DO NOT Display the command-line that started a process. If this option is not present, then command-line information is displayed.

Options can also be negated by simply typing another dash after the specified option.

Some examples of what some of the options could look like and their outputs:
```
$ ./TUps -p 1400 -S -v
1400: utime=16 stime=2 vmsize=88224 [/user/libexec/gsd-power]
```
```
$ ./TUps -s -S -v -c
996: S utime=8 stime=6 vmsize=78349
997: R utime=45 stime=7 vmsize=23458
1000: R utime=34 stime=10 vmsize=92823
1250: R utime=12 stime=12 vmsize=12743
1331: S utime=7 stime=5 vmsize=42891
1400: S utime=16 stime=2 vmsize=88224
...
```