cmake_minimum_required(VERSION 3.8)

add_library(mmThread STATIC mmThread.c)
set_property(TARGET mmThread PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmThread mmLogSimple mmStructure pthread)
