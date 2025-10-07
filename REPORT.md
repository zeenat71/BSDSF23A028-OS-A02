# Feature 4: Alphabetical Sort (ls-v1.4.0) - REPORT

### 1. Why is it necessary to read all directory entries into memory before you can sort them? What are the potential drawbacks of this approach for directories containing millions of files?

It is necessary to read all directory entries into memory because sorting requires having access to all the elements at once. Functions like `qsort()` cannot sort elements on disk one by one; they need a complete array of filenames to compare and rearrange them alphabetically.

**Potential drawbacks:**
- **High memory usage:** For directories with millions of files, storing all filenames in memory can consume a large amount of RAM.
- **Performance issues:** Allocating and managing memory for huge arrays can slow down the program.
- **Risk of failure:** If the system runs out of memory, the program may crash or fail to sort the directory.

---

### 2. Explain the purpose and signature of the comparison function required by `qsort()`. How does it work, and why must it take `const void *` arguments?

**Purpose:**  
The comparison function tells `qsort()` how to compare two elements in the array. For alphabetical sorting, it compares two filenames and determines their relative order.

**Signature:**
```c
int cmpfunc(const void *a, const void *b);
