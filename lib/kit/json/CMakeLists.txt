cmake_minimum_required(VERSION 3.8)

add_library(mmJson STATIC mmJsonE.c mmJson.c)
set_property(TARGET mmJson PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmJson mmLogSimple)
