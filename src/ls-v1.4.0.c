/*
* Programming Assignment 02: ls-v1.4.0
* Features:
* - Default: column display (down then across)
* - -x : horizontal (across) display
* - Alphabetical sort of filenames
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPACING 2

int display_mode = 0; // 0 = down-across, 1 = horizontal

// Safe comparison function for qsort
int cmpfunc(const void *a, const void *b) {
    const char *fa = *(const char **)a;
    const char *fb = *(const char **)b;
    return strcmp(fa, fb);
}

// Get terminal width
int get_terminal_width() {
    struct winsize w;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
        return 80; // fallback
    return w.ws_col;
}

// Vertical display: down then across
void print_down_across(char **files, int n) {
    int maxlen = 0;
    for(int i=0;i<n;i++)
        if((int)strlen(files[i]) > maxlen) maxlen = strlen(files[i]);

    int term_width = get_terminal_width();
    int cols = term_width / (maxlen + SPACING);
    if(cols < 1) cols = 1;
    int rows = (n + cols - 1) / cols;

    for(int r=0;r<rows;r++){
        for(int c=0;c<cols;c++){
            int idx = c*rows + r;
            if(idx<n)
                printf("%-*s", maxlen+SPACING, files[idx]);
        }
        printf("\n");
    }
}

// Horizontal display: row-major
void print_horizontal(char **files, int n) {
    int maxlen = 0;
    for(int i=0;i<n;i++)
        if((int)strlen(files[i]) > maxlen) maxlen = strlen(files[i]);

    int term_width = get_terminal_width();
    int col_width = maxlen + SPACING;
    int cur_width = 0;

    for(int i=0;i<n;i++){
        if(cur_width + col_width > term_width){
            printf("\n");
            cur_width = 0;
        }
        printf("%-*s", col_width, files[i]);
        cur_width += col_width;
    }
    printf("\n");
}

// Read, sort, and print directory
void do_ls(const char *dir) {
    DIR *dp = opendir(dir);
    if(!dp){
        fprintf(stderr,"Cannot open directory: %s\n", dir);
        return;
    }

    struct dirent *entry;
    int capacity = 100, count = 0;
    char **files = malloc(sizeof(char*)*capacity);

    while((entry = readdir(dp)) != NULL){
        if(entry->d_name[0] == '.') continue; // skip hidden
        if(count>=capacity){
            capacity *= 2;
            files = realloc(files, sizeof(char*)*capacity);
        }
        files[count++] = strdup(entry->d_name);
    }
    closedir(dp);

    // Alphabetical sort
    qsort(files, count, sizeof(char*), cmpfunc);

    // Display
    if(display_mode == 0)
        print_down_across(files, count);
    else
        print_horizontal(files, count);

    // Free memory
    for(int i=0;i<count;i++) free(files[i]);
    free(files);
}

int main(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "x")) != -1){
        if(opt == 'x') display_mode = 1;
    }

    if(optind == argc){ // no directory
        do_ls(".");
    } else {
        for(int i=optind;i<argc;i++){
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i]);
            printf("\n");
        }
    }

    return 0;
}
