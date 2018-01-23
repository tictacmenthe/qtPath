cmake_minimum_required(VERSION 3.8)

project(qtstar)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*/*.cpp" "src/*/*.h" "src/*.cpp" "src/*.h")
add_executable(qtstar ${SOURCES} src/Graph/Edge.cpp src/Graph/Edge.h src/Graph/Movable.cpp src/Graph/Movable.h)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts Gui)
target_link_libraries(qtstar Qt5::Core Qt5::Widgets Qt5::Charts Qt5::Gui)