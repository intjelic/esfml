# ------------------------------------------------------------------------------
#  BlackBerry CMake toolchain file, for use with BlackBerry NDKs
#  Requires cmake 2.8.3 or newer.
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
#  On Linux, you may have to run the cmake command twice because of a CMake not 
#  being able to complete compiler tests.
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

cmake_minimum_required(VERSION 2.8.3)

# on some platforms (OSX) this may not be defined, so search for it
if(NOT DEFINED CMAKE_INSTALL_NAME_TOOL)
    find_program(CMAKE_INSTALL_NAME_TOOL install_name_tool)
endif()

# subsequent toolchain loading is not really needed
if(DEFINED CMAKE_CROSSCOMPILING)
    return()
endif()

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

# we want to add the ".exe" suffix if we are on windows
set(OS_SUFFIX "")
if(CMAKE_HOST_WIN32)
    set(OS_SUFFIX ".exe")
endif()

# define the architecture name and codename
if(BLACKBERRY_TARGET STREQUAL "device")
    set(ARCHITECTURE_CODENAME "ntoarmv7le")
    set(ARCHITECTURE_NAME     "armle-v7")
elseif(BLACKBERRY_TARGET STREQUAL "simulator")
    set(ARCHITECTURE_CODENAME "ntox86")
    set(ARCHITECTURE_NAME     "x86")
endif()

# define qcc flags according the chosen options
set(QCC_EXECUTABLE "${BLACKBERRY_QNX_HOST}/usr/bin/qcc${OS_SUFFIX}")
set(QCC_VERSION    "-Vgcc_${ARCHITECTURE_CODENAME}")
set(QCC_STL        "-Y ${BLACKBERRY_STL}")

# set up the cross compiler
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER   ${QCC_EXECUTABLE} CACHE PATH "C compiler.")
set(CMAKE_CXX_COMPILER ${QCC_EXECUTABLE} CACHE PATH "CXX compiler.")
list(APPEND CMAKE_FIND_ROOT_PATH "${BLACKBERRY_QNX_TARGET}")
list(APPEND CMAKE_FIND_ROOT_PATH "${BLACKBERRY_QNX_TARGET}/${ARCHITECTURE_NAME}")

# search for libraries and includes in the ndk only
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# define unneeded stuff for cmake to be happy
set(CMAKE_STRIP   "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-strip${OS_SUFFIX}"   CACHE PATH "Path to a program.")
set(CMAKE_AR      "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-ar${OS_SUFFIX}"      CACHE PATH "Path to a program.")
set(CMAKE_LINKER  "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-ld${OS_SUFFIX}"      CACHE PATH "Path to a program.")
set(CMAKE_NM      "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-nm${OS_SUFFIX}"      CACHE PATH "Path to a program.")
set(CMAKE_OBJCOPY "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-objcopy${OS_SUFFIX}" CACHE PATH "Path to a program.")
set(CMAKE_OBJDUMP "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-objdump${OS_SUFFIX}" CACHE PATH "Path to a program.")
set(CMAKE_RANLIB  "${BLACKBERRY_QNX_HOST}/usr/bin/${ARCHITECTURE_CODENAME}-ranlib${OS_SUFFIX}"  CACHE PATH "Path to a program.")

# add blakberry preprocessor definitions
add_definitions(-D__PLAYBOOK__)
add_definitions(-D__QNXNTO__)

# global compilation flags
set(CMAKE_C_FLAGS   "${QCC_VERSION} ${QCC_STL} -lang-c")
set(CMAKE_CXX_FLAGS "${QCC_VERSION} ${QCC_STL} -lang-c++")

# release and debug compilation flags
set(CMAKE_C_FLAGS_RELEASE   "-O2")
set(CMAKE_C_FLAGS_DEBUG     "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")

# release and debug linker flags
set(CMAKE_STATIC_LINKER_FLAGS         "${QCC_VERSION} ${QCC_STL}")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUG   "")
set(CMAKE_SHARED_LINKER_FLAGS         "${QCC_VERSION} ${QCC_STL}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "")

# cache flags
set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}"           CACHE STRING "Flags used by the compiler during all build types.")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}"   CACHE STRING "Flags used by the compiler during release builds.")
set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}"     CACHE STRING "Flags used by the compiler during debug builds.")

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}"         CACHE STRING "Flags used by the compiler during all build types." )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Flags used by the compiler during release builds.")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"   CACHE STRING "Flags used by the compiler during debug builds.")

set(CMAKE_STATIC_LINKER_FLAGS         "${CMAKE_STATIC_LINKER_FLAGS}"         CACHE STRING "Flags used by the linker during the creation of static libraries.")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during release builds.")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUG   "${CMAKE_STATIC_LINKER_FLAGS_DEBUG}"   CACHE STRING "Flags used by the linker during debug builds.")

set(CMAKE_SHARED_LINKER_FLAGS         "${CMAKE_SHARED_LINKER_FLAGS}"         CACHE STRING "Flags used by the linker during the creation of shared libraries.")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" CACHE STRING "Flags used by the linker during release builds.")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}"   CACHE STRING "Flags used by the linker during debug builds.")

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

# global flag for cmake client scripts to change behavior
set(BLACKBERRY 1)
