
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/video)

# all source files
set(SRC ${SRCROOT}/Video.cpp)

# define the sound target
sfml_add_example(video
                 SOURCES ${SRC}
                 DEPENDS sfml-video sfml-graphics sfml-window sfml-system)
