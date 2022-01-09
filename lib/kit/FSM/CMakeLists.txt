cmake_minimum_required(VERSION 3.8)

add_library(mmFSM STATIC mmFSM.c)
set_property(TARGET mmFSM PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmFSM mmLogSimple)
