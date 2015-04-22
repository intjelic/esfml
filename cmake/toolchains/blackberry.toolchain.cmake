# ------------------------------------------------------------------------------
#  BlackBerry CMake toolchain file, for use with BlackBerry NDKs
#  Requires cmake 3.2.1 or newer.
#
#  This toolchain was designed to be used by the SFML project but you can reuse
#  it for your own project.
#
#  This toolchain doesn't support "release minsize" and "release with debug
#  info" build as well as the creation of module or executable. Only static and
#  shared build in either release or debug mode are allowed.
#
#  This toolchain was written based on the following documents:
#   - https://developer.blackberry.com/native/reference/core/com.qnx.doc.neutrino.utilities/topic/q/qcc.html
#   - http://developer.blackberry.com/native/documentation/core/porting_getting_started.html
#
#  Usage Linux & Mac:
#   $ source /absolute/path/to/the/bbndk/bbndk-env.sh
#   $ mkdir build
#   $ cd build
#   $ cmake -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain/blackberry.toolchain.cmake" ..
#   $ make -j8
#
#  Usage Windows:
#   > /absolute/path/to/the/bbndk/bbndk-env.bat
#   > mkdir build
#   > cd build
#   > cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/toolchain/blackberry.toolchain.cmake" ..
#   > make -j8
#
#  Important note regarding the MinGW Makefiles generator: it won't work because
#  it doesn't want any sh.exe defined in the path environment variable. You
#  need to remove them as well as the one included in the Blackberry NDK.
#
#  Options (can be set as cmake parameters: -D<option_name>=<value>):
#
#   * BLACKBERRY_TARGET="device" - Specifies the target type
#
#       Can be set to build for either an existing device (arm) or  a Blackberry
#       simulator (x86).
#
#   Possible targets are:
#     "device"   - For existing devices (default)
#     "simulator - For Blackberry simulators
#
#   * BLACKBERRY_STL="_cpp" - Specifies the STL implementation
#
#      Can be set to select the STL implementation to link against.
#
#   Possible STL are:
#     _gpp     - GNU C++ library (default)
#     _cpp     - Dinkum C++ library
#     _cpp-ne  - Dinkum C++ library (no exceptions)
#     _acpp    - Dinkum Abridged C++ library
#     _acpp-ne - Dinkum Abridged C++ library (no exceptions)
#     _ecpp    - Embedded Dinkum C++ library
#     _ecpp-ne - Embedded Dinkum C++ library (no exceptions)

cmake_minimum_required(VERSION 3.2.1)

# set default option value
if(NOT DEFINED BLACKBERRY_TARGET)
    set(BLACKBERRY_TARGET "device")
endif()
if(NOT DEFINED BLACKBERRY_STL)
    set(BLACKBERRY_STL "_gpp")
endif()

# retrieve environment info set by bbndk-env(.sh/.bat) script
set(BLACKBERRY_QNX_HOST   "$ENV{QNX_HOST}")
set(BLACKBERRY_QNX_TARGET "$ENV{QNX_TARGET}")

# define the architecture name and codename
if(BLACKBERRY_TARGET STREQUAL "device")
    set(ARCHITECTURE_CODENAME "ntoarmv7le")
    set(ARCHITECTURE_NAME     "armle-v7")
elseif(BLACKBERRY_TARGET STREQUAL "simulator")
    set(ARCHITECTURE_CODENAME "ntox86")
    set(ARCHITECTURE_NAME     "x86")
endif()

# set up the cross compiler
set(CMAKE_SYSTEM_NAME QNX)
set(CMAKE_C_COMPILER qcc)
set(CMAKE_C_COMPILER_TARGET gcc_${ARCHITECTURE_CODENAME})
set(CMAKE_CXX_COMPILER QCC)
set(CMAKE_CXX_COMPILER_TARGET gcc_${ARCHITECTURE_CODENAME})
list(APPEND CMAKE_FIND_ROOT_PATH "${BLACKBERRY_QNX_TARGET}")
list(APPEND CMAKE_FIND_ROOT_PATH "${BLACKBERRY_QNX_TARGET}/${ARCHITECTURE_NAME}")

# search for headers and libraries in the ndk only
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# add blakberry preprocessor definitions
add_definitions(-D__QNXNTO__)

# cache flags
set( CMAKE_CXX_FLAGS "" CACHE STRING "Flags used by the compiler during all build types.s" )
set( CMAKE_C_FLAGS   "" CACHE STRING "Flags used by the compiler during all build types." )

# blackberry compilation flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Y ${BLACKBERRY_STL} -lang-c")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Y ${BLACKBERRY_STL} -lang-c++")

# macro to find packages on the host OS
macro(find_host_package)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)

    if(CMAKE_HOST_WIN32)
        set(WIN32)
        set(UNIX)
    elseif(CMAKE_HOST_APPLE)
        set(APPLE)
        set(UNIX)
    endif()

    find_package(${ARGN})

    set(WIN32)
    set(APPLE)
    set(UNIX)

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro()
