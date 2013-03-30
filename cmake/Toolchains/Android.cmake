# As Android native applications are compiled by the NDK and so, use its build 
# system (mandatory) which contains many toolchains, we must fool CMake to 
# obtain the same build interface. Here, we're setting non-existing toolchains 
# to satisfy CMake. Only CMAKE_SYSTEM_NAME is relevant as it will identify the 
# targetted platfrom across our CMake scripts.
include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 0)
set(CMAKE_SYSTEM_PROCESSOR unknown)

CMAKE_FORCE_C_COMPILER(/ GNU)
CMAKE_FORCE_CXX_COMPILER(/ GNU)
