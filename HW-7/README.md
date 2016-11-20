# HW 7 #

## 8.9 ##

Of course, only if the running time of two processes are overlapped, they can run concurrently.

- AB: No
- AC: Yes
- AD: Yes
- BC: Yes
- BD: Yes
- CD: Yes

## 8.11 ##

The father process forks to two processes when i = 0, then the two processes fork to 4 when i = 1. So there will be 4 processes after the for loop, and there will be 4 `hello` outputs.

## 8.12 ##

The same as 8.13, after the two forks in `doit`, there will be 4 processes, since all the processes copies the stack data, so the return will work properly as the father process.

So each process outputs 2 `hello`, and there are 8 `hello` outputs altogether.

## 8.18 ##

Of course there are 4 processes altogether. According to the return value of 2 forks the 4 processes will output:

- 1
- 0
- 12
- 02

These strings can be combined randomly but should maintain its order.

In the case, A, C, E are possible outputs.
