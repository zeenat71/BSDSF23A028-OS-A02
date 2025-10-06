/*
* Programming Assignment 02: lsv1.3.0
* Horizontal Column Display (-x), Down Then Across (default), and Long Listing (-l)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

extern int errno;

void do_ls(const char *dir, int display_mode);
void down_then_across(char **files, int count, int max_len);
void horizontal_display(char **files, int count, int max_len);
void long_listing(const char *dir);

int main(int argc, char *argv[])
{
    int display_mode = 0; // 0=default, 1=-l, 2=-x
    int opt;

    while((opt = getopt(argc, argv, "lx")) != -1) {
        switch(opt) {
            case 'l': display_mode = 1; break;
            case 'x': display_mode = 2; break;
            default: fprintf(stderr, "Usage: %s [-l|-x] [directory...]\n", argv[0]); exit(EXIT_FAILURE);
        }
    }

    if(optind == argc) { // no directory given
        do_ls(".", display_mode);
    } else {
        for(int i = optind; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i], display_mode);
            puts("");
        }
    }

    return 0;
}

// Main directory reading function
void do_ls(const char *dir, int display_mode)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if(dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    // Count files and find longest filename
    int count = 0;
    int max_len = 0;
    char **files = NULL;

    while((entry = readdir(dp)) != NULL) {
        if(entry->d_name[0] == '.') continue; // skip hidden
        files = realloc(files, sizeof(char*) * (count+1));
        files[count] = strdup(entry->d_name);
        int len = strlen(entry->d_name);
        if(len > max_len) max_len = len;
        count++;
    }
    closedir(dp);

    // Call correct display function
    if(display_mode == 1) { // -l
        long_listing(dir);
    } else if(display_mode == 2) { // -x horizontal
        horizontal_display(files, count, max_len);
    } else { // default down then across
        down_then_across(files, count, max_len);
    }

    // Free allocated memory
    for(int i = 0; i < count; i++) free(files[i]);
    free(files);
}

// Down then across display (vertical)
void down_then_across(char **files, int count, int max_len)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int spacing = 2;
    int cols = term_width / (max_len + spacing);
    if(cols < 1) cols = 1;
    int rows = (count + cols - 1)/cols;

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int idx = c*rows + r;
            if(idx < count)
                printf("%-*s", max_len + spacing, files[idx]);
        }
        printf("\n");
    }
}

// Horizontal display (-x)
void horizontal_display(char **files, int count, int max_len)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int term_width = w.ws_col;
    int spacing = 2;
    int col_width = max_len + spacing;
    int cur_width = 0;

    for(int i = 0; i < count; i++) {
        if(cur_width + col_width > term_width) {
            printf("\n");
            cur_width = 0;
        }
        printf("%-*s", col_width, files[i]);
        cur_width += col_width;
    }
    printf("\n");
}

// Simple long listing (-l)
void long_listing(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if(dp == NULL) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    while((entry = readdir(dp)) != NULL) {
        if(entry->d_name[0] == '.') continue;

        struct stat st;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);
        if(lstat(path, &st) == -1) continue;

        printf("%10ld %s\n", st.st_size, entry->d_name); // simple output, can extend with perms/user/group
    }

    closedir(dp);
}
