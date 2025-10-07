3. Report Questions

Q1: Why read all directory entries into memory before sorting?

Sorting requires access to all filenames at once.

Drawbacks:

High memory usage for directories with millions of files.

May slow down for very large directories.

Risk of crashing if memory is insufficient.

Q2: Purpose and signature of comparison function for qsort()

Purpose: Define how two elements (filenames) are compared.

Signature: int cmpfunc(const void *a, const void *b)

How it works:

Cast to char ** and compare using strcmp().

Return <0, 0, >0 based on alphabetical order.

const void * allows qsort() to be generic and ensures array elements are not modified.
