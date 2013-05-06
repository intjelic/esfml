
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Video)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Video)

# all source files
set(SRC
    ${SRCROOT}/VideoFile.cpp
    ${SRCROOT}/VideoFile.hpp
    ${SRCROOT}/VideoBuffer.cpp
    ${INCROOT}/VideoBuffer.hpp
    ${SRCROOT}/VideoStream.cpp
    ${INCROOT}/VideoStream.hpp
    ${SRCROOT}/Movie.cpp
    ${INCROOT}/Movie.hpp
    ${INCROOT}/Video.hpp
)

source_group("" FILES ${SRC})

find_package(FFMPEG REQUIRED)
add_definitions(-D__STDC_CONSTANT_MACROS)

# define the sfml-window target
sfml_add_library(sfml-video
                 SOURCES ${SRC}
                 DEPENDS ${FFMPEG_LIBRARIES} sfml-system)
