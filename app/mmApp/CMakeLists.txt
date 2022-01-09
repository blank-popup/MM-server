cmake_minimum_required(VERSION 3.8)

add_executable(mmmApp mmmApp.c mmmAppExt.c mmmAppTask.c mmmAppPipe.c mmmAppSOCK.c mmmAppSHM.c mmmAppMQ.c mmmAppJson.c)
set_target_properties(mmmApp PROPERTIES ENABLE_EXPORTS TRUE)
target_link_libraries(mmmApp ${LIBS_MM} ${LIBS_DEFAULT})

add_library(mmmLibApp STATIC mmmAppExt.c mmmAppTask.c mmmAppPipe.c mmmAppSOCK.c mmmAppSHM.c mmmAppMQ.c mmmAppJson.c)
set_property(TARGET mmmLibApp PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})

add_executable(mmRun mmRun.c mmRunExt.c mmRunTask.c)
set_target_properties(mmRun PROPERTIES ENABLE_EXPORTS TRUE)
# target_link_libraries(mmRun ${LIBS_MM} ${LIBS_DEFAULT})
target_link_libraries(mmRun mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})

add_subdirectory(so)
add_subdirectory(plugin)

add_custom_command(
    TARGET mmmApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app/mmApp/config ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}/config
    COMMAND ${CMAKE_COMMAND} -E copy mmmApp ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}/mmmApp
)

add_custom_command(
    TARGET mmRun POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy mmRun ${CMAKE_SOURCE_DIR}/${DIRECTORY_RUN}/mmRun
)
