cmake_minimum_required(VERSION 3.8)

add_library(subThread SHARED subThread.c subThreadExt.c subThreadTask.c)
set_property(TARGET subThread PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(subThread mmmPlugin rest mmmServerVariable mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})
# target_link_libraries(subThread mmmPlugin rest mmmServerVariable mmmLibApp mmLibRun ${LIBS_MM} ${LIBS_DEFAULT})

add_custom_command(
    TARGET subThread POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${DIRECTORY_BUILD_SO_APP}/subThread/libsubThread.so ${CMAKE_SOURCE_DIR}/${DIRECTORY_SO}/libsubThread.so
)
