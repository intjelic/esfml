
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/user_interface)

# all source files
set(SRC ${SRCROOT}/user_interface.cpp)

# define the pong target
sfml_add_example(user_interface GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-gui sfml-graphics sfml-window sfml-system)
