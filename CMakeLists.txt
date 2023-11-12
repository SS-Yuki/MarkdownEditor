cmake_minimum_required(VERSION 3.15.0)
project(Markdown)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
AUX_SOURCE_DIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/src" SRC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
add_executable(markdown ${SRC})