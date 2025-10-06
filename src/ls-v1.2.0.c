#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

extern int errno;

void do_ls(const char *dir)
{
    struct dirent *entry;
    DIR *dp = opendir(dir);
    if (!dp) {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    char **filenames = NULL;
    int count = 0, max_len = 0;

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        filenames = realloc(filenames, sizeof(char*) * (count + 1));
        filenames[count] = strdup(entry->d_name);

        int len = strlen(entry->d_name);
        if (len > max_len) max_len = len;

        count++;
    }
    closedir(dp);

    if (count == 0) return;

    struct winsize w;
    int spacing = 2;
    int term_width = 80; // fallback width
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) term_width = w.ws_col;

    int num_cols = term_width / (max_len + spacing);
    if (num_cols == 0) num_cols = 1;
    int num_rows = (count + num_cols - 1) / num_cols;

    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            int idx = r + c * num_rows;
            if (idx < count)
                printf("%-*s", max_len + spacing, filenames[idx]);
        }
        printf("\n");
    }

    for (int i = 0; i < count; i++) free(filenames[i]);
    free(filenames);
}
int main(int argc, char const *argv[])
{
    if (argc == 1) {
        do_ls(".");
    } else {
        for (int i = 1; i < argc; i++) {
            printf("Directory listing of %s : \n", argv[i]);
            do_ls(argv[i]);
            puts("");
        }
    }
    return 0;
}
