cmake_minimum_required(VERSION 3.8)

add_library(mmBase64 STATIC base64.c)
set_property(TARGET mmBase64 PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmBase64 mmLogSimple)
