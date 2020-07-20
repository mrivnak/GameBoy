CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

PROJECT(GameBoy)

SET(CMAKE_CXX_STANDARD 17)

SET(SOURCE_FILES
	src/application.cpp
#   src/audio.cpp
	src/bitmap.cpp
	src/display.cpp
	src/main.cpp
	src/memory-bus.cpp
	src/motherboard.cpp
	src/processor.cpp
	src/registers.cpp
	src/render-device.cpp
	src/instructions.cpp
#   src/termdebug.cpp
	src/texture.cpp
	src/window.cpp
)

SET(TEST_SOURCE_FILES

)

INCLUDE_DIRECTORIES(${INCLUDE_DIRS} C:/cpplibs/include C:/clibs/include ./include)
LINK_DIRECTORIES(C:/cpplibs/lib)

ADD_EXECUTABLE(GameBoy ${SOURCE_FILES})
TARGET_LINK_lIBRARIES(GameBoy ${LIBRARIES} openal GL SDL2)

# ADD_EXECUTABLE(GameBoyTests ${SOURCE_FILES} ${TEST_SOURCE_FILES})
# TARGET_LINK_lIBRARIES(GameBoyTests ${LIBRARIES} openal GL SDL2)
