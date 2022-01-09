cmake_minimum_required(VERSION 3.8)

add_library(mmURL STATIC mmURL.c)
set_property(TARGET mmURL PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmURL mmLogSimple mmJson)
