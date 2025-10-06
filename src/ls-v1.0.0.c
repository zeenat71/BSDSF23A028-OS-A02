/*
* Programming Assignment 02: lsv1.0.0
* Modified for Feature-2: ls-v1.1.0 - Complete Long Listing Format
* Adds -l option for long listing format
* Usage:
*       $ lsv1.1.0 
*       % lsv1.1.0 -l
*       $ lsv1.1.0 /home -l
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

extern int errno;

// Function declarations
void simple_listing(const char *dir);
void long_listing(const char *dir);

int main(int argc, char const *argv[])
{
    int long_format = 0; // Flag for -l option
    int opt;
    int start_index = 1;

    // Parse command-line options
    while ((opt = getopt(argc, (char * const *)argv, "l")) != -1)
    {
        switch(opt)
        {
            case 'l':
                long_format = 1;
                start_index++; // skip -l in directory loop
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory...]\n", argv[0]);
                return 1;
        }
    }

    // If no directories provided, use "."
    if (argc == start_index)
    {
        if (long_format)
            long_listing(".");
        else
            simple_listing(".");
    }
    else
    {
        for (int i = start_index; i < argc; i++)
        {
            printf("Directory listing of %s:\n", argv[i]);
            if (long_format)
                long_listing(argv[i]);
            else
                simple_listing(argv[i]);
            puts("");
        }
    }

    return 0;
}

// Original simple listing (your old do_ls() code)
void simple_listing(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory : %s\n", dir);
        return;
    }
    errno = 0;
    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        printf("%s\n", entry->d_name);
    }

    if (errno != 0)
    {
        perror("readdir failed");
    }

    closedir(dp);
}

// New long listing function for -l
void long_listing(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (dp == NULL)
    {
        fprintf(stderr, "Cannot open directory : %s\n", dir);
        return;
    }

    while ((entry = readdir(dp)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        struct stat file_stat;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

        if (stat(path, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        // File type
        char type = '-';
        if (S_ISDIR(file_stat.st_mode)) type = 'd';
        else if (S_ISLNK(file_stat.st_mode)) type = 'l';

        // Permissions
        char perms[10];
        perms[0] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
        perms[1] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
        perms[2] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
        perms[3] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
        perms[4] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
        perms[5] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
        perms[6] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
        perms[7] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
        perms[8] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
        perms[9] = '\0';

        // Owner and group
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);

        // Modification time
        char timebuf[80];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&file_stat.st_mtime));

        // Print long listing line
        printf("%c%s %ld %s %s %ld %s %s\n",
               type, perms, file_stat.st_nlink,
               pw ? pw->pw_name : "?", gr ? gr->gr_name : "?",
               file_stat.st_size, timebuf,
               entry->d_name);
    }

    closedir(dp);
}
