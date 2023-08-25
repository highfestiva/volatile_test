# volatile_test

A tiny and simple test (written in C++) for volatile int contention between CPUs. An atomic_int is used as a
synchronization primitive.

Two threads simultaneously loop ten times and try to increase the volatile counter, and race conditions ensue.
Sometimes the final count will be 20, but often it will be less.


## Example output on Intel i7-12850HX, 2100 Mhz, 16 cores, 24 CPUs

```bash
$ ./build-run.sh
result  hits
0       0
1       0
2       0
3       1
4       2
5       6
6       10
7       8
8       9
9       14
10      3049181
11      176292
12      438441
13      2261718
14      2438862
15      43057
16      23614
17      296018
18      477901
19      351906
20      442960
```
