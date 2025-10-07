# REPORT — Feature 7: Recursive Listing (-R)

### 1. Base Case in a Recursive Function
- A **base case** is a condition that stops recursion from continuing indefinitely.  
- In the context of our recursive `ls`, the base case occurs when:
  - A directory contains **no subdirectories**, or
  - The entry is `.` or `..` (these are skipped to prevent looping back).  
- Without a base case, the function would call itself infinitely and crash.

### 2. Importance of Constructing a Full Path
- Before making a recursive call, we construct the **full path** of a subdirectory:
- This ensures that the recursive call correctly accesses the intended directory.  

- **What happens if you don’t?**  
- If we just call `do_ls("subdir")` from within `do_ls("parent_dir")`:
  - The program will look for `subdir` in the **current working directory**, not in `parent_dir`.
  - This may cause “cannot open directory” errors if the working directory is not `parent_dir`.
  - Recursion will fail to correctly traverse nested directories.
