cmake_minimum_required(VERSION 3.8)

add_library(mmLogSimple STATIC mmLogSimple.c)
set_property(TARGET mmLogSimple PROPERTY POSITION_INDEPENDENT_CODE TRUE)

add_subdirectory(base)
add_subdirectory(kit)
