# HW 9 #

## 10.6 ##

fd2 = 4

We know that when `open` is called, system will find a unused file descriptor. When `baz.txt` is opened, file descriptor 0, 1, 2 refer to `stdin`, `stdout` and `stderr`, and 3 refers to `foo.txt` (`bar.txt` is closed). So 4 is the least unused file descriptor.

## 10.9 ##

As we all know, shell uses `dup2` to implement I/O redirection. The father process forks and then open the input file, which gets a file descriptor 3, and then call `dup2` to copy it to fd 0 before calling `execve`.

However, when we refer to fd 3 in the child process, it gives "bad file descriptor". It means that the child process must have closed fd 3 before `execve` as it is unnecessary. So the code should be:

```C++
if (fork() == 0) {
    int fd = open("foo.txt", O_RDONLY, 0);
    dup2(fd, 0);
    close(fd);

    execve("fstatcheck", argv, envp);
}
```

## 10.10 ##

Just add redirection before the read & write (before line 9):

```C++
if (argc > 1) {
    int fd = open(argv[1], O_RDONLY, 0);
    dup2(fd, 0);
    close(fd);
}
```
