#!/bin/bash
CC=gcc
CCFLAGS="-g -Wall"
OBJ=tmp/test

$CC $CCFLAGS -o $OBJ $1 && $OBJ && rm -f -r ./tmp/*
