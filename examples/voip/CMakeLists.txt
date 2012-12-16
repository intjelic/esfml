
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/voip)

# all source files
set(SRC ${SRCROOT}/voip.cpp
        ${SRCROOT}/client.cpp
        ${SRCROOT}/server.cpp)

# define the voip target
sfml_add_example(voip
                 SOURCES ${SRC}
                 DEPENDS sfml-audio sfml-network sfml-system)
