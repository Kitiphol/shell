cmake_minimum_required(VERSION 4.00)
project(icsh CXX)

set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(icsh src/icsh.c++)
