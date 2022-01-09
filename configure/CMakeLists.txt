cmake_minimum_required(VERSION 3.8)

add_executable(mmConfigure mmConfigure.c mmConfigureExt.c mmConfigureTask.c)
set_target_properties(mmConfigure PROPERTIES ENABLE_EXPORTS TRUE)
target_link_libraries(mmConfigure subThread mmmPlugin rest mmmServerVariable mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})
# target_link_libraries(mmConfigure subThread mmmPlugin rest mmmServerVariable mmmLibApp mmLibRun ${LIBS_MM} ${LIBS_DEFAULT})

add_custom_command(
    TARGET mmConfigure POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy mmConfigure ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}/mmConfigure
)
