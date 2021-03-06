/* procfs_pidmax.c

   This program demonstrates how to access a file in the /proc file system. It
   can be used to read and modify the /proc/sys/kernel/pid_max file (which is
   available only in Linux 2.6 and later).

   Usage: procfs_pidmax [new-pidmax]

        Displays the current maximum PID, and, if a command line
        argument is supplied, sets the maximum PID to that value.

        Note: privilege is required to change the maximum PID value.

   This program is Linux-specific.
*/
#include <fcntl.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>

#define MAX_LINE 100
int
main(int argc, char *argv[])
{
    int fd;
    char line[MAX_LINE];
    ssize_t n;

    fd = open("/proc/fork_info", O_RDONLY);
    if (fd == -1)
        exit(EXIT_FAILURE);

    n = read(fd, line, MAX_LINE);
    if (n == -1)
        exit(EXIT_FAILURE);

    printf("%.*s", (int) n, line);


    // TODO: want to read until it finishes (probably failure?)
    // Then parser to get the values and format them to make it look nicer

    exit(EXIT_SUCCESS);
}