#!/bin/bash

DESTDIR=$PWD/tmp

PATH=$PWD/toolchains/mips/bin:$PATH
CC=mipsel-linux-android-gcc
CXX=mipsel-linux-android-g++
CFLAGS=-I$DESTDIR/usr/include
CPPFLAGS=-I$DESTDIR/usr/include
LDFLAGS=-L$DESTDIR/usr/lib

./compile_libs.sh mips $PATH $CC $CXX $CFLAGS $CPPFLAGS $LDFLAGS
