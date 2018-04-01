cmake_minimum_required(VERSION 3.8)

project(qtstar)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "src/*.cpp" "src/*.h")

find_package(Qt5Widgets REQUIRED)

add_executable(qtstar ${SOURCES})

target_link_libraries(qtstar Qt5::Widgets)