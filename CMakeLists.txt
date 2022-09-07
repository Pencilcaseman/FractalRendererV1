cmake_minimum_required(VERSION 3.10)
project(FractalRenderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(FractalRenderer main.cpp)

set(LIBRAPID_USE_MULTIPREC on)
add_subdirectory(cinderbox)

target_link_libraries(FractalRenderer PUBLIC cinderbox)
