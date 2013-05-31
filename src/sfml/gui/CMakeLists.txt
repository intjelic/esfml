
set(INCROOT ${PROJECT_SOURCE_DIR}/include/sfml/gui)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sfml/gui)

# all source files
set(SRC
    ${INCROOT}/export.hpp
    ${SRCROOT}/GuiWindow.cpp
    ${INCROOT}/GuiWindow.hpp
)
source_group("" FILES ${SRC})

# define the sfml-audio target
sfml_add_library(sfml-gui
                 SOURCES ${SRC}
                 DEPENDS sfml-system sfml-window sfml-graphics
                 EXTERNAL_LIBS "")
