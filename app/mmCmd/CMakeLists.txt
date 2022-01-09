cmake_minimum_required(VERSION 3.8)

add_executable(mmCmd mmCmd.c mmCmdExt.c mmCmdTask.c)
target_link_libraries(mmCmd mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})

install (TARGETS mmCmd
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/${DIRECTORY_SO}
    ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/${DIRECTORY_PLUGIN_SO}
)

add_custom_command(
    TARGET mmCmd POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy mmCmd ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}/mmCmd
)
