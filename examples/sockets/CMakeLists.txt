
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/sockets)

# all source files
set(SRC ${SRCROOT}/sockets.cpp
        ${SRCROOT}/tcp.cpp
        ${SRCROOT}/udp.cpp)

# define the sockets target
sfml_add_example(sockets
                 SOURCES ${SRC}
                 DEPENDS sfml-network sfml-system)
