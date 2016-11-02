#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cachelab.h"

#define infi 0x7FFFFFFF

typedef struct {
    char valid;
    unsigned tag;
    int dirty;
} Line;

Line *cache;
int s, E, b, clock;
int hits, misses, evictions;
char vv;

FILE *in;

#define line(i, j) (cache + (i) * E + (j))

void initSet() {
    cache = malloc(sizeof(Line) * (1 << s) * E);
    memset(cache, 0, sizeof(Line) * (1 << s) * E);
}

void freeSet() {
    free(cache);
    cache = NULL;
}

void load(unsigned set, unsigned tag) {
    int l, recent = infi;
    Line *now;
    for (int i = 0; i < E; ++i) {
        now = line(set, i);
        if (!now -> valid) {
            l = i;
            break;
        } else if (now -> dirty < recent) {
            recent = now -> dirty;
            l = i;
        }
    }
    now = line(set, l);
    if (now -> valid) {
        if (vv) printf(" %s", "eviction");
        ++evictions;
    }
    now -> valid = 1;
    now -> tag = tag;
    now -> dirty = clock;
}

void access(unsigned address) {
    unsigned tag = address >> (s + b);
    unsigned set = (address >> b) & ((1 << s) - 1);
    int i;
    char hit = 0;
    for (i = 0; i < E; ++i) {
        Line *now = line(set, i);
        if (now -> valid && now -> tag == tag) {
            now -> dirty = clock;
            hit = 1;
            break;
        }
    }
    if (vv) printf(" %s", hit ? "hit" : "miss");
    hits += hit;
    misses += !hit;
    if (!hit) load(set, tag);
    ++clock;
}

int pre(int argc, char **argv) {
    const char *path = NULL;
    int i;
    s = b = E = 0;
    clock = 0;
    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'v':
                    vv = 1;
                    break;
                case 's':
                    if (i == argc - 1) return 0;
                    s = atoi(argv[++i]);
                    break;
                case 'b':
                    if (i == argc - 1) return 0;
                    b = atoi(argv[++i]);
                    break;
                case 'E':
                    if (i == argc - 1) return 0;
                    E = atoi(argv[++i]);
                    break;
                case 't':
                    if (i == argc - 1) return 0;
                    path = argv[++i];
                    break;
                default:
                    return 0;
            }
        } else {
            return 0;
        }
    }
    in = fopen(path, "r");
    return in && s && b && E;
}

void work() {
    initSet();
    char buf[256];
    while (fgets(buf, 256, in)) {
        if (buf[0] == ' ') {
            unsigned address, nothing;
            char x;
            sscanf(buf + 1, "%c %x,%d", &x, &address, &nothing);
            if (vv) printf("%c %x,%d", x, address, nothing);
            access(address);
            if (x == 'M') access(address);
            if (vv) printf("\n");
        }
    }
    printSummary(hits, misses, evictions);
    freeSet();
}

void printHelp() {
    puts("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>");
    puts("Options:");
    puts("  -h         Print this help message.\n");
    puts("  -v         Optional verbose flag.");
    puts("  -s <num>   Number of set index bits.");
    puts("  -E <num>   Number of lines per set.");
    puts("  -b <num>   Number of block offset bits.");
    puts("  -t <file>  Trace file.");
}

int main(int argc, char **argv) {
    if (pre(argc, argv)) {
        work();
    } else {
        printHelp();
    }

    return 0;
}
