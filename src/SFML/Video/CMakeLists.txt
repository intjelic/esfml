
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Video)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Video)

# all source files
set(SRC
    ${SRCROOT}/VideoFile.cpp
    ${SRCROOT}/VideoFile.hpp
)

source_group("" FILES ${SRC})

# define the sfml-window target
sfml_add_library(sfml-video
                 SOURCES ${SRC}
                 DEPENDS sfml-system)
