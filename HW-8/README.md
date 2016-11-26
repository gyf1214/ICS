# HW 8 #

## 8.16 ##

counter = 2;

Because the father process and the child process have their own memory space respectively, so the counter of the two processes are different and won't interfere with each other.

## 8.21 ##

fork() ----> "b" ----> waitpid() ----> "c"
        |               |
        ---> "a" --------

The possible output sequence is the topological sequence of the flow graph:

- abc
- bac

## 8.25 ##

See `tfgets.c`.

Note that it is not thread safe, so if there are concurrent operations (including other signals), it won't work properly.
