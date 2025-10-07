/*
 * Programming Assignment 02: ls-v1.5.0
 * Feature: Colorized Output Based on File Type
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define SPACING 2

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_RED "\033[1;31m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_REVERSE "\033[7m"

int display_mode = 0; // 0 = down-across, 1 = horizontal

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

// Function to print filename with color
void print_colored_name(const char *dir, const char *filename) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, filename);

    struct stat st;
    if (lstat(path, &st) == -1) {
        printf("%s%s\n", COLOR_RESET, filename);
        return;
    }

    const char *color = COLOR_RESET;

    if (S_ISDIR(st.st_mode))
        color = COLOR_BLUE; // Directory
    else if (S_ISLNK(st.st_mode))
        color = COLOR_MAGENTA; // Symbolic Link
    else if (S_ISREG(st.st_mode) && (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
        color = COLOR_GREEN; // Executable
    else if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode) || S_ISSOCK(st.st_mode))
        color = COLOR_REVERSE; // Special Files
    else if (strstr(filename, ".tar") || strstr(filename, ".gz") || strstr(filename, ".zip"))
        color = COLOR_RED; // Archives

    printf("%s%s%s", color, filename, COLOR_RESET);
}

void print_down_across(char **files, int n, const char *dir) {
    int maxlen = 0;
    for (int i = 0; i < n; i++)
        if ((int)strlen(files[i]) > maxlen) maxlen = strlen(files[i]);
    int term_width = get_terminal_width();
    int cols = term_width / (maxlen + SPACING);
    if (cols < 1) cols = 1;
    int rows = (n + cols - 1) / cols;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int idx = c * rows + r;
            if (idx < n) {
                print_colored_name(dir, files[idx]);
                int spaces = maxlen + SPACING - strlen(files[idx]);
                for (int s = 0; s < spaces; s++) printf(" ");
            }
        }
        printf("\n");
    }
}

void print_horizontal(char **files, int n, const char *dir) {
    int maxlen = 0;
    for (int i = 0; i < n; i++)
        if ((int)strlen(files[i]) > maxlen) maxlen = strlen(files[i]);
    int term_width = get_terminal_width();
    int col_width = maxlen + SPACING;
    int cur_width = 0;

    for (int i = 0; i < n; i++) {
        if (cur_width + col_width > term_width) {
            printf("\n");
            cur_width = 0;
        }
        print_colored_name(dir, files[i]);
        int spaces = col_width - strlen(files[i]);
        for (int s = 0; s < spaces; s++) printf(" ");
        cur_width += col_width;
    }
    printf("\n");
}

void do_ls(const char *dir) {
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "Cannot open directory %s: %s\n", dir, strerror(errno));
        return;
    }

    struct dirent *entry;
    int capacity = 100, count = 0;
    char **files = malloc(sizeof(char *) * capacity);

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        if (count >= capacity) {
            capacity *= 2;
            files = realloc(files, sizeof(char *) * capacity);
        }
        files[count++] = strdup(entry->d_name);
    }

    closedir(dp);
    qsort(files, count, sizeof(char *), cmpfunc);

    if (display_mode == 0)
        print_down_across(files, count, dir);
    else
        print_horizontal(files, count, dir);

    for (int i = 0; i < count; i++) free(files[i]);
    free(files);
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "x")) != -1) {
        if (opt == 'x') display_mode = 1;
    }

    if (optind == argc)
        do_ls(".");
    else {
        for (int i = optind; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i]);
            printf("\n");
        }
    }

    return 0;
}
