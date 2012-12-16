
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/win32)

# all source files
set(SRC ${SRCROOT}/win32.cpp)

# define the win32 target
sfml_add_example(win32 GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system)
