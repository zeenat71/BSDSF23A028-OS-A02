#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#define SPACING 2

void do_ls(const char *dir);

int main(int argc, char const *argv[])
{
    if (argc == 1) {
        do_ls(".");
    } else {
        for (int i = 1; i < argc; i++) {
            printf("Directory listing of %s:\n", argv[i]);
            do_ls(argv[i]);
            puts("");
        }
    }
    return 0;
}

void do_ls(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    // Step 2: Gather all filenames
    char **files = NULL;
    size_t count = 0, capacity = 10;
    files = malloc(capacity * sizeof(char*));
    size_t longest_len = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        if (count >= capacity) {
            capacity *= 2;
            files = realloc(files, capacity * sizeof(char*));
        }
        files[count] = strdup(entry->d_name);
        size_t len = strlen(entry->d_name);
        if (len > longest_len) longest_len = len;
        count++;
    }
    closedir(dp);

    if (count == 0) {
        free(files);
        return;
    }

    // Step 3: Terminal width and column layout
    struct winsize w;
    int term_width = 80; // default
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
        term_width = w.ws_col;
    }

    int num_cols = term_width / (longest_len + SPACING);
    if (num_cols == 0) num_cols = 1;
    int num_rows = (count + num_cols - 1) / num_cols;

    // Step 4: Down then across printing
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            int idx = col * num_rows + row;
            if (idx < count) {
                printf("%-*s", (int)(longest_len + SPACING), files[idx]);
            }
        }
        printf("\n");
    }

    // Free memory
    for (size_t i = 0; i < count; i++) free(files[i]);
    free(files);
}
