# CSC415-P2
## Rewrite fileStats.c so that each input text file is handled by a different POSIX thread. Suppose there are N input text files to be counted. Follow these steps:

## Main thread creates N concurrent threads Each thread:

### computes counts for one file writes its counts to a FileInfo struct prints its thread ID and its counts to stdout

### Main thread waits for each thread to exit, reads the FileInfo struct from the exiting thread, and accumulates counts. When all threads are done, the main thread reports the totals.

### Make sure there are N concurrent threads! Otherwise points will be deducted.

### Make sure that your code works with any reasonable N. That is, if you need an array with N elements, allocate the array dynamically, not statically.
