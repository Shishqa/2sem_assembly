cmake_minimum_required(VERSION 3.16)
project(05_img_blend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -msse4.2 -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer")

add_executable(05_img_blend main.cpp Blender.cpp Blender.h Image.cpp Image.h)