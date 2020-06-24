CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

PROJECT(GameBoy)

SET(CMAKE_CXX_STANDARD 17)

SET(SOURCE_FILES
    src/main.cpp
    src/application.cpp
    src/window.cpp
    src/render-device.cpp
    src/registers.cpp
    src/memory-bus.cpp
    src/processor.cpp
    src/instructions.cpp
    src/termdebug.cpp
)

INCLUDE_DIRECTORIES(${INCLUDE_DIRS} C:/cpplibs/include C:/clibs/include ./include)
LINK_DIRECTORIES(C:/cpplibs/lib)

ADD_EXECUTABLE(GameBoy ${SOURCE_FILES})
TARGET_LINK_lIBRARIES(GameBoy ${LIBRARIES} openal GL SDL2)
