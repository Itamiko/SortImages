cmake_minimum_required(VERSION 3.16)
project(sortImages LANGUAGES C VERSION 0.0.1)

set(CMAKE_C_STANDARD 11)

# libpng
include(cmake/libpng.cmake)

add_executable(si src/main.c)
target_link_libraries(si png)
