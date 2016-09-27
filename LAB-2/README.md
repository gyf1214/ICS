## LAB-2 Notes ##

Fuck tao@ubuntu64!

## how to debug offline ##

- change hostname to one of ics9, ics10, ... ics16

- create an http server on your machine (or host machine)

- change hosts to resolve ics16.pku.edu.cn to your fake server

> note: a default server.rb written in ruby & sinatra is provided. It is recommended to use a virtual machine with host-only network.

## phase 1 ##

    const char s[];

input: str

check: strcmp(str, s) == 0;

## phase 2 ##

input: a[1] a[2] ... a[6]

check: a[i] is Fib, where a1 == 0 && a2 == 1 && a[i + 2] = a[i + 1] + a[i];

## phase 3 ##

    const int a[8];

input: x y (0 <= x <= 7)

check: y == a[x]

note: it uses switch/case

## phase 4 ##

    const int a;

    int func4(int edi, int esi, int edx) {
        int ebx = (edx + esi) / 2;
        int eax;
        if (ebx > edi) eax = ebx + func4(edi, esi, ebx - 1);
        if (ebx < edi) eax = ebx + func4(edi, ebx + 1, edx);
        eax = ebx;
        return eax;
    }

input: x y (0 <= x <= 14)

check: func(x, 0, 14) == a && y == a

## phase 5 ##

    const int a[16];
    const int b;

    int func(str) {
        int ecx = 0;
        for (int eax = 0; eax <= 6; ++eax) {
            unsigned edx = str[eax];
            edx = edx & 0xf;
            ecx += a[edx];
        }
    }

input: str (strlen(str) == 6);

check: func(str) == b

## phase 6 ##

    struct Node {
        int x;
        int id;
        node \*next;
    }node[6];

    node[i] -> next = &node[i + 1];
    node[5] -> next = NULL;
    node[i] -> id = i;

input: a[1] a[2] ... a[6] ({ a[i] } = {1, 2, 3, 4, 5, 6})

check: node[7 - a[i]] -> x is not decremental

## secret phase

    struct Node {
        int x;
        node \*ch[2];
    }node[];

    // node gives a complete binary search tree with height = 4
    // that is: this -> ch[0] -> x < this -> x && this -> ch[1] -> x > this -> x

    int func7(Node \*t, int x) {
        if (!t) return -1;
        if (t -> x > x) return 2 * func7(x -> ch[0], x);
        if (t -> x < x) return 2 * func7(x -> ch[1], x) + 1;
        return 0;
    }

    const int b;

how: In phase 4, give an additional string in the input "tao@ubuntu64", then input a new line.

input: x (1 <= x <= 1001)

check: func7(node, x) == b
