#!/usr/bin/env bash

LOCALDIR=$PWD
DESTDIR=$PWD/tmp

if [ "$BLACKBERRY_NDK" == "" ]
then
	echo "Where is the NDK location ?"
	echo -n "NDK: "; read NDK
	export BLACKBERRY_NDK=$NDK
fi

rm -r $PWD/build
./download_sources.sh

source $BLACKBERRY_NDK/bbndk-env_10_2_0_1155.sh

RANLIB="${QNX_HOST}/usr/bin/ntoarmv7-ranlib "
CPP="${QNX_HOST}/usr/bin/qcc -V4.6.3,gcc_ntoarmv7le -E "
CC="${QNX_HOST}/usr/bin/qcc -V4.6.3,gcc_ntoarmv7le "
LD="${QNX_HOST}/usr/bin/ntoarmv7-ld "
CPPFLAGS="-D__PLAYBOOK__ -D__QNXNTO__ "
CFLAGS=" -g -fPIC -fstack-protector-strong"
LDFLAGS="-L${QNX_TARGET}/armle-v7/lib -L${PREFIX}/lib -lscreen -lasound -lpps -lm -lpng14 -lbps -lEGL -lGLESv2 -Wl,-z,relro -Wl,-z,now -pie" 

PARAMS="--host=arm-unknown-nto-qnx6.5.0eabi --prefix=$DESTDIR/device/usr"

# Compile OGG
cd $LOCALDIR/build/libogg-* && ./configure $PARAMS && make -j8 && make install
rm $DESTDIR/device/usr/lib/libogg*.so*

# Compile FLAC
cd $LOCALDIR/build/flac-* && ./configure $PARAMS && make -j8 && make install
rm $DESTDIR/device/usr/lib/libFLAC*.so*

# Compile VORBIS
cd $LOCALDIR/build/libvorbis-* && ./configure $PARAMS && make -j8 && make install
rm $DESTDIR/device/usr/lib/libvorbis*.so*

# Compile libsndfile (important: --disable-sqlite)
cd $LOCALDIR/build/libsndfile-* && ./configure $PARAMS --disable-sqlite && make && make install
