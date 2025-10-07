/*
* Programming Assignment 02: ls-v1.6.0
* Feature: Recursive Listing (-R Option)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SPACING 2

int display_mode = 0;      // 0 = down-across
int recursive_flag = 0;    // 1 = recursive (-R)

int cmpfunc(const void *a, const void *b) {
    char *str1 = *(char **)a;
    char *str2 = *(char **)b;
    return strcmp(str1, str2);
}

int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 80;
    return w.ws_col;
}

void print_down_across(char **files, int n) {
    int maxlen = 0;
    for (int i = 0; i < n; i++)
        if ((int)strlen(files[i]) > maxlen)
            maxlen = strlen(files[i]);

    int term_width = get_terminal_width();
    int cols = term_width / (maxlen + SPACING);
    if (cols < 1) cols = 1;
    int rows = (n + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = c * rows + r;
            if (idx < n)
                printf("%-*s", maxlen + SPACING, files[idx]);
        }
        printf("\n");
    }
}

void do_ls(const char *dir);

void process_directory(const char *dir) {
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    struct dirent *entry;
    int capacity = 100;
    int count = 0;
    char **files = malloc(sizeof(char *) * capacity);

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue; // skip hidden
        if (count >= capacity) {
            capacity *= 2;
            files = realloc(files, sizeof(char *) * capacity);
        }
        files[count++] = strdup(entry->d_name);
    }
    closedir(dp);

    qsort(files, count, sizeof(char *), cmpfunc);

    printf("\n%s:\n", dir);
    print_down_across(files, count);

    if (recursive_flag) {
        for (int i = 0; i < count; i++) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dir, files[i]);

            struct stat st;
            if (lstat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                if (strcmp(files[i], ".") != 0 && strcmp(files[i], "..") != 0)
                    do_ls(path);
            }
        }
    }

    for (int i = 0; i < count; i++)
        free(files[i]);
    free(files);
}

void do_ls(const char *dir) {
    process_directory(dir);
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "xR")) != -1) {
        switch (opt) {
            case 'x': display_mode = 1; break;
            case 'R': recursive_flag = 1; break;
        }
    }

    if (optind == argc)
        do_ls(".");
    else {
        for (int i = optind; i < argc; i++)
            do_ls(argv[i]);
    }

    return 0;
}
