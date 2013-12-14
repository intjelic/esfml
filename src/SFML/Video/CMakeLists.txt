
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Video)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Video)

# all source files
set(SRC
    ${SRCROOT}/VideoPlayer.cpp
    ${SRCROOT}/VideoPlayer.hpp
    ${SRCROOT}/VideoFile.cpp
    ${SRCROOT}/VideoFile.hpp
    ${INCROOT}/VideoSource.hpp
    ${SRCROOT}/VideoSource.cpp
    ${SRCROOT}/VideoBuffer.cpp
    ${INCROOT}/VideoBuffer.hpp
    ${SRCROOT}/VideoStream.cpp
    ${INCROOT}/VideoStream.hpp
    ${SRCROOT}/Movie.cpp
    ${INCROOT}/Movie.hpp
    ${SRCROOT}/Video.cpp
    ${INCROOT}/Video.hpp
)

source_group("" FILES ${SRC})

find_package(FFMPEG REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_definitions(-D__STDC_CONSTANT_MACROS)

# define the sfml-window target
sfml_add_library(sfml-video
                 SOURCES ${SRC}
                 DEPENDS sfml-system
                 EXTERNAL_LIBS ${FFMPEG_LIBRARIES} ${OpenCV_LIBS} swscale avformat)
