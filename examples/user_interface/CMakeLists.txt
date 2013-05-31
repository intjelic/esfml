
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/user_interface)

# all source files
set(SRC ${SRCROOT}/user_interface.cpp)

# find external libraries
find_package(GTK2 REQUIRED)

# add include paths of external libraries
include_directories(${GTK2_INCLUDE_DIRS})

# define the pong target
sfml_add_example(user_interface GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-gui sfml-graphics sfml-window sfml-system ${GTK2_LIBRARIES})
