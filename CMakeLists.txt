cmake_minimum_required(VERSION 3.22)
project(SteganografiaObrazowa)

set(CMAKE_CXX_STANDARD 17)

add_executable(SteganografiaObrazowa main.cpp BMP.h getBytes.h PNG.h PNG.cpp BMP.cpp getBytes.cpp GIF.h GIF.cpp Image.h)
