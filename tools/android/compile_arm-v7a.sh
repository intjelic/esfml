#!/bin/bash

DESTDIR=$PWD/tmp

PATH=$PWD/toolchains/arm/bin:$PATH
CC=arm-linux-androideabi-gcc
CXX=arm-linux-androideabi-g++
CFLAGS=$CFLAGS:\ -march=armv7-a\ -mfloat-abi=softfp\ -mfpu=vfpv3-d16
CPPFLAGS=$CPPFLAGS:\ -march=armv7-a\ -mfloat-abi=softfp\ -mfpu=vfpv3-d16
LDFLAGS=$LDFLAGS:\ -march=armv7-a\ -Wl,--fix-cortex-a8

./compile_libs.sh arm $PATH $CC $CXX $CFLAGS $CPPFLAGS $LDFLAGS
