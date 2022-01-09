cmake_minimum_required(VERSION 3.8)

add_library(mmBase STATIC mmIPC.c mmSocket.c mmMultiplex.c mmMemory.c mmString.c mmTime.c mmFile.c mmEtc.c mmBaseType.c)
set_property(TARGET mmBase PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmBase mmLogSimple)
