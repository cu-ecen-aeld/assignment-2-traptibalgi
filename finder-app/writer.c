/*******************************************************************************
 * Copyright (C) 2024 by Trapti Damodar Balgi
 *
 * Redistribution, modification or use of this software in source or binary
 * forms is permitted as long as the files maintain this copyright. Users are
 * permitted to modify this and use it to learn about the field of embedded
 * software. <Student Name> and the University of Colorado are not liable for
 * any misuse of this material.
 * ****************************************************************************/

/**
 * @file    writer.c
 * @brief   writer function to write a string to a file
 *
 * @author  Trapti Damodar Balgi
 * @date    09/08/2024
 * @references  
 * 1. https://linux.die.net/man/3/syslog
 * 2. https://linux.die.net/man/2/stat
 * 3. https://man7.org/linux/man-pages/man3/fprintf.3p.html
 * 4. AESD Lectures and Slides
 *
 */

#include<syslog.h>
#include<stdio.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

#define ERROR (1)

int main(int argc, char *argv[]) 
{
    // Open "writer" syslog along with PID and LOG_USER facility
    openlog("writer", LOG_PID, LOG_USER);

    // If the correct number of arguments not specified, exit with an error
    if (argc != 3) {
        syslog(LOG_ERR, "Expecting 2 parameters!");
        printf("Usage: ./%s <directory-path> <string-to-be-searched>\n\r", argv[0]);
        closelog();
        return ERROR;
    }

    // Extract the CL arguments
    const char *filesdir = argv[1];
    const char *writestr = argv[2];

    char filesdir_copy[strlen(filesdir) + 1];
    strcpy(filesdir_copy, filesdir);
    const char *directory_name = dirname(filesdir_copy);

    // Lines 61 and 68 were partially generated using ChatGPT at https://chat.openai.com/ with prompts including [How to check if a directory exists in a c program using file io]

    struct stat info;

    // If filesdir is not a directory, exit with an error
    if (stat(directory_name, &info) != 0) 
    {
        // Directory does not exist
        syslog(LOG_ERR, "Directory does not exist! Please enter a valid directory path");
        printf("ERROR: Invalid path!\n\r");
        closelog();
        return ERROR;
    } 
    else if (!(info.st_mode & S_IFDIR))
    {
        // Path exists but is not a directory
        syslog(LOG_ERR, "Path exists but is not a directory! Please enter a valid directory path\n\r");
        printf("ERROR: Invalid path!\n\r");
        closelog();
        return ERROR;
    }

    // Creates file if doesn't already exist. If creation fails, exit with an error
    FILE *writefile = fopen(filesdir, "w");
    if (writefile == NULL) 
    {
        syslog(LOG_ERR, "ERROR: Could not open file %s: %s", filesdir, strerror(errno));
        printf("ERROR: Could not open file!\n\r");
        closelog();
        return ERROR;
    }

    // Write the string to the file. If returns a negative number, write was not succesful, exit with an error
    if (fprintf(writefile, "%s", writestr) < 0) 
    {
        syslog(LOG_ERR, "ERROR: Could not write to file %s: %s", filesdir, strerror(errno));
        printf("ERROR: Could not write to file!\n\r");
        fclose(writefile);
        closelog();
        return ERROR;
    }

    // Log if succesful on LOG_DEBUG level
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, filesdir);

    // Close the file
    fclose(writefile);

    // Close the log
    closelog();

    return 0;
}