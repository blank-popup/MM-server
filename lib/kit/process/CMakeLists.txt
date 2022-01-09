cmake_minimum_required(VERSION 3.8)

add_library(mmProcess STATIC mmProcess.c)
set_property(TARGET mmProcess PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmProcess mmLogSimple)
