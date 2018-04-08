cmake_minimum_required(VERSION 3.8)

project(qtstar)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*/*.cpp" "src/*/*.h" "src/*.cpp" "src/*.h")
add_executable(qtstar ${SOURCES})
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(qtstar Qt5::Core Qt5::Widgets)