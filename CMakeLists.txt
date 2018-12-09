cmake_minimum_required(VERSION 3.8)
project(datalog-interpreter-cs236-byu)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror=sign-compare -g")

file(GLOB SOURCES "./src/*")
add_executable(datalog-interpreter-cs236-byu ${SOURCES})