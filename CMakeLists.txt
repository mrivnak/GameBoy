cmake_minimum_required(VERSION 3.7)

project(GameBoy)

set(CMAKE_CXX_STANDARD 17)

include_directories(${INCLUDE_DIRS} C:/cpplibs/include)
link_directories(C:/cpplibs/lib)

add_executable(GameBoy src/main.cpp src/window.cpp src/registers.cpp src/memory-bus.cpp src/processor.cpp src/instruction.cpp)
target_link_libraries(GameBoy ${LIBRARIES} GL SDL2)
