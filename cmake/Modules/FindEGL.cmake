#
# Try to find EGL library and include path.
# Once done this will define
#
# EGL_FOUND
# EGL_INCLUDE_PATH
# EGL_LIBRARY
#
# Alternatively, it can find an emulator if the variable GLES_EMULATOR 
# is defined. To do so, you will have to define its paths
#
# EGL_EMULATOR_INCLUDE_PATH
# EGL_EMULATOR_LIBRARY
# 

if(NOT EGL_EMULATOR)
	find_path(EGL_INCLUDE_DIR EGL/egl.h)
	find_library(EGL_LIBRARY NAMES EGL)
		
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(EGL DEFAULT_MSG EGL_LIBRARY EGL_INCLUDE_DIR)
else()
	set(EGL_FOUND 1)
	set(EGL_INCLUDE_PATH ${EGL_EMULATOR_INCLUDE_PATH})
	set(EGL_LIBRARY ${EGL_EMULATOR_LIBRARY})
endif()
