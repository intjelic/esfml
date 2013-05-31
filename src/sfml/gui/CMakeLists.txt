
set(INCROOT ${PROJECT_SOURCE_DIR}/include/sfml/gui)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sfml/gui)

# all source files
set(SRC
    ${INCROOT}/export.hpp
    ${SRCROOT}/GuiWindow.cpp
    ${INCROOT}/GuiWindow.hpp
    ${SRCROOT}/Widget.cpp
    ${INCROOT}/Widget.hpp
    ${SRCROOT}/Button.cpp
    ${INCROOT}/Button.hpp
)
source_group("" FILES ${SRC})

# find external libraries
find_package(GTK2 REQUIRED)

# add include paths of external libraries
include_directories(${GTK2_INCLUDE_DIRS})

# define the sfml-audio target
sfml_add_library(sfml-gui
                 SOURCES ${SRC}
                 DEPENDS sfml-graphics sfml-window sfml-system
                 EXTERNAL_LIBS ${GTK2_LIBRARIES})
