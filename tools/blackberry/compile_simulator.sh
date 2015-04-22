#!/usr/bin/env bash

LOCALDIR=$PWD
DESTDIR=$PWD/tmp
PREFIX=$DESTDIR/simulator/usr

if [ "$BLACKBERRY_NDK" == "" ]
then
	echo "Where is the NDK location ?"
	echo -n "NDK: "; read NDK
	export BLACKBERRY_NDK=$NDK
fi

rm -r $PWD/build
./download_sources.sh

source $BLACKBERRY_NDK/bbndk-env.sh

RANLIB="${QNX_HOST}/usr/bin/ntox86-ranlib "
CC="${QNX_HOST}/usr/bin/qcc -V4.8.2,gcc_ntox86 "
CPP="${QNX_HOST}/usr/bin/qcc -V4.8.2,gcc_ntox86 -E "
LD="${QNX_HOST}/usr/bin/ntox86-ld "
CXXFLAGS="-fPIC "
CPPFLAGS="-D__QNXNTO__ -I${PREFIX}/include "
CFLAGS="-g -fPIC -I${PREFIX}/include "
LDFLAGS="-L${QNX_TARGET}/x86/lib -L${PREFIX}/lib -lscreen -lasound -lpps -lm -lpng14 -lbps -lEGL -lGLESv2 -Wl,-z,relro -Wl,-z,now "
export PATH RANLIB CC CPP LD CXXFLAGS CPPFLAGS CFLAGS LDFLAGS

PARAMS="--host=i486-pc-nto-qnx8.0.0 --prefix=$PREFIX --enable-shared=no"

# Compile OGG
cd $LOCALDIR/build/libogg-* && ./configure $PARAMS && make -j8 && make install

# Compile FLAC
cd $LOCALDIR/build/flac-* && ./configure $PARAMS --disable-cpplibs && make -j8 && make install

# Compile VORBIS
cd $LOCALDIR/build/libvorbis-* && ./configure $PARAMS && make -j8 && make install
