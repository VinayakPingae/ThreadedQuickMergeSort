CS 451/551 Systems Programming, Fall 2014

Out: 11/15/2014 Sat.
Due: 11/26/2014 Wed. 23:59:59

Plagarism Notice: Do not copy paste. Please Refer.

ThreadedQuickMergeSort
======================

In this task, the code to complete the program that performs a parallel merge sort using Pthreads. The input data is a 
bunch of integers. The program divides the input data into a number of splits according to user indication. After that, the 
program first performs a quick sort on each split, then it merges every two sorted splits into a larger sorted split until 
there is only one split (i.e., all the integers have been sorted).


Multi-Threaded Quick and Merge Sort

