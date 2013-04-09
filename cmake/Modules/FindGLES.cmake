#
# Try to find GLES library and include path.
# Once done this will define
#
# GLES_FOUND
# GLES_INCLUDE_PATH
# GLES_LIBRARY
#
# Alternatively, it can find an emulator if the variable GLES_EMULATOR 
# is defined. To do so, you will have to define its paths
#
# GLES_EMULATOR_INCLUDE_PATH
# GLES_EMULATOR_LIBRARY
# 

if(NOT GLES_EMULATOR)
	find_path(GLES_INCLUDE_DIR GLES/gl.h)
	find_library(GLES_LIBRARY NAMES ELGSv1_CM)
		
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(GLES DEFAULT_MSG GLES_LIBRARY GLES_INCLUDE_DIR)
else()
	set(GLES_FOUND 1)
	set(GLES_INCLUDE_PATH ${GLES_EMULATOR_INCLUDE_PATH})
	set(GLES_LIBRARY ${GLES_EMULATOR_LIBRARY})
endif()
