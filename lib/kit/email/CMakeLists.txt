cmake_minimum_required(VERSION 3.8)

add_library(mmEmail STATIC email.c)
set_property(TARGET mmEmail PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmEmail mmLogSimple mmBase64 ssl crypto)
