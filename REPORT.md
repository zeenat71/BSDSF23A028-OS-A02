# Report – Feature 6: ls-v1.5.0 (Colorized Output)

## Question 1:
**How do ANSI escape codes work to produce color in a standard Linux terminal? Show the specific code sequence for printing text in green.**

ANSI escape codes are special character sequences that tell the terminal how to format or color the output.  
They always start with the escape character `\033` (or `\x1B`), followed by `[`, then a color or style code, and finally `m`.  

**General format:**

When printed to the terminal, this sequence changes the color or style of text until the reset code is used.  

**Common examples:**
| Color | Code | Description |
|--------|------|-------------|
| Red | `\033[31m` | Red text |
| Green | `\033[32m` | Green text |
| Yellow | `\033[33m` | Yellow text |
| Blue | `\033[34m` | Blue text |
| Reset | `\033[0m` | Return to normal color |

**Example in C:**
```c
printf("\033[32mThis text appears in green!\033[0m\n");
Question 2:

To color an executable file, you need to check its permission bits. Explain which bits in the st_mode field you need to check to determine if a file is executable by the owner, group, or others.

Each file in Linux has permissions stored in its st_mode field, which is part of the struct stat structure (from <sys/stat.h>).
This field contains bits that define read, write, and execute permissions for the owner, group, and others.

To check if a file is executable, we use these predefined macros:

Permission	Macro	Description
Owner Execute	S_IXUSR	Executable by file owner
Group Execute	S_IXGRP	Executable by group
Others Execute	S_IXOTH	Executable by others
struct stat st;
stat(filename, &st);

if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH) {
    printf("\033[32m%s\033[0m\n", filename); // Green for executable
}
