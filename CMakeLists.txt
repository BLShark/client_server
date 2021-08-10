cmake_minimum_required(VERSION 2.8)

project(server)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package( Threads )
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "*.cpp" "*.h" "*.hpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
