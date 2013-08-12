# We need to know where the NDK is located. We'll define some options to 
# customize the build process using NDK/ndk-build: a library might be built for 
# multiple architecture targetting different API level at the same time.

# See the NDK as an entire filesystem on its own where we would install headers 
# and libraries in it.

# Attempt to get the NDK path by reading typicial environement variable name
if(NOT $ENV{NDK} STREQUAL "")
	set(NDK_PATH $ENV{NDK})
elseif(NOT $ENV{NDK_PATH} STREQUAL "")
	set(NDK_PATH $ENV{NDK_PATH})
elseif(NOT $ENV{ANDROID_NDK} STREQUAL "")
	set(NDK_PATH $ENV{ANDROID_NDK})
else()
	set(NDK_PATH /path/to/the/NDK)
endif()

# The most important, the NDK location.
sfml_set_option(ANDROID_NDK_PATH ${NDK_PATH} STRING "Where is the NDK located")

# By default, it targets the most widespread and used. 
sfml_set_option(ANDROID_API_LEVEL 9 STRING "What API level should it target")

# A library might be compiled for four different architecture
sfml_set_option(ANDROID_ABI_ARM TRUE BOOL "Build for arm architecture ?")
sfml_set_option(ANDROID_ABI_ARMv7 FALSE BOOL "Build for armv7 architecture ?")
sfml_set_option(ANDROID_ABI_MIPS FALSE BOOL "Build for mipsel architecture ?")
sfml_set_option(ANDROID_ABI_x86 FALSE BOOL "Build for x86 architecture ?")

set(CMAKE_INSTALL_PREFIX ${ANDROID_NDK_PATH}/sources/sfml)
