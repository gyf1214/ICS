CC=gcc
CCFLAGS="-g -Wall"

($CC $CCFLAGS -o tmp/test $1) && tmp/test && rm -f -r ./tmp/*
