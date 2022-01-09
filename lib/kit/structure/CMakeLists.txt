cmake_minimum_required(VERSION 3.8)

add_library(mmStructure STATIC mmCircularLinkedList.c mmTree.c mmStack.c mmQueue.c mmLinkedList.c)
set_property(TARGET mmStructure PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmStructure mmLogSimple)
