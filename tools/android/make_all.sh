#!/bin/bash

# Abort if no known installation of NDK
if [ "$NDK" == "" ]
then
	echo "Where is the NDK location ?"
	echo -n "NDK: "; read NDK
	export NDK=$NDK
fi

./clean_all.sh

./create_toolchains.sh
./download_sources.sh
./compile_arm.sh
./compile_x86.sh
./compile_mips.sh
./compile_arm-v7a.sh
