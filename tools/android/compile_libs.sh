#!/bin/bash

DESTDIR=$PWD/tmp
LOCALDIR=$PWD

OPATH=$PATH

export PATH=$2
export CC=$3
export CXX=$4
export CFLAGS=$5
export CPPFLAGS=$6
export LDFLAGS=$7

if [ "$1" = "arm" ]
then
	ARCH=arm-linux

elif [ "$1" = "arm-v7a" ]
then
	ARCH=arm-linux

elif [ "$1" = "x86" ]
then
	ARCH=i686-linux

elif [ "$1" = "mips" ]
then
	ARCH=mips-linux
fi

HOST="--host=$ARCH"
PREFIX="--prefix=$DESTDIR/$1/usr"

# Compile OGG
cd $LOCALDIR/build/libogg-* && ./configure $HOST $PREFIX && make && make install

# Compile FLAC
cd  $LOCALDIR/build/flac-* && ./configure $HOST $PREFIX && make && make install

# Compile VORBIS
cd  $LOCALDIR/build/libvorbis-* && ./configure $HOST $PREFIX  && make && make install

# Compile libsndfile (important: --disable-sqlite)
cd  $LOCALDIR/build/libsndfile-* && ./configure $HOST $PREFIX --disable-sqlite && make && make install

# Compile JPEG
cd $LOCALDIR/build/jpeg-* && ./configure $HOST $PREFIX && make && make install

# Compile freetype
cd  $LOCALDIR/build/freetype-* && ./configure $HOST $PREFIX && make && make install

# todo: compile OpenAL-Soft

export PATH=$OPATH
