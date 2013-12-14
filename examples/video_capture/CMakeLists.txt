
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/video_capture)

# all source files
set(SRC ${SRCROOT}/VideoCapture.cpp)

# define the video-capture target
sfml_add_example(video-capture
                 SOURCES ${SRC}
                 DEPENDS sfml-video sfml-graphics sfml-window sfml-system)
