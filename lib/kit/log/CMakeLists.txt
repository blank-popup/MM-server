cmake_minimum_required(VERSION 3.8)

add_library(mmLog STATIC mmLog.c)
set_property(TARGET mmLog PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmLog mmLogSimple mmEmail mmJson mmStructure)
