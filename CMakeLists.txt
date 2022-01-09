cmake_minimum_required(VERSION 3.8)

project(MM)

set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

set(LIBS_DEFAULT dl rt pthread)
set(LIBS_MM mmEmail mmBase64 mmStructure mmProcess mmThread mmFSM mmJson mmLog mmBase)

set(DIRECTORY_BUILD_SO build/app/mmApp/so)
set(DIRECTORY_BUILD_SO_APP ${DIRECTORY_BUILD_SO}/app)
set(DIRECTORY_BUILD_PLUGIN build/app/mmApp/plugin)
set(DIRECTORY_RUN run)
set(DIRECTORY_SO ${DIRECTORY_RUN}/so)
set(DIRECTORY_PLUGIN_SO ${DIRECTORY_RUN}/plugin/so)
set(DIRECTORY_PLUGIN_PY ${DIRECTORY_RUN}/plugin/py)

include_directories(lib)

function(build_lib_so name_so)
    add_library(${name_so} SHARED ${ARGN})
    set_property(TARGET ${name_so} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    target_link_libraries(${name_so} mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})
    # target_link_libraries(${name_so} mmmLibApp mmLibRun ${LIBS_MM} ${LIBS_DEFAULT})
endfunction()

function(build_app_so name_so)
    add_library(${name_so} SHARED ${ARGN})
    set_property(TARGET ${name_so} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    target_link_libraries(${name_so} rest mmmServerVariable mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})
    # target_link_libraries(${name_so} rest mmmServerVariable mmmLibApp mmLibRun ${LIBS_MM} ${LIBS_DEFAULT})

    set(name_file lib${name_so}.so)

    add_custom_command(
        TARGET ${name_so} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${DIRECTORY_BUILD_SO_APP}/${name_so}/${name_file} ${CMAKE_SOURCE_DIR}/${DIRECTORY_SO}/${name_file}
    )
endfunction()

function(build_plugin_so name_so)
    add_library(${name_so} SHARED ${ARGN})
    set_property(TARGET ${name_so} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    target_link_libraries(${name_so} subThread mmmPlugin rest mmmServerVariable mmmLibApp ${LIBS_MM} ${LIBS_DEFAULT})
    # target_link_libraries(${name_so} subThread mmmPlugin rest mmmServerVariable mmmLibApp mmLibRun ${LIBS_MM} ${LIBS_DEFAULT})

    set(name_file lib${name_so}.so)

    add_custom_command(
        TARGET ${name_so} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${DIRECTORY_BUILD_PLUGIN}/${name_so}/${name_file} ${CMAKE_SOURCE_DIR}/${DIRECTORY_PLUGIN_SO}/${name_file}
    )
endfunction()

function(copy_plugin_py name_file)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${DIRECTORY_PLUGIN_PY})
    file(COPY ${name_file} DESTINATION ${CMAKE_SOURCE_DIR}/${DIRECTORY_PLUGIN_PY})
endfunction()

add_subdirectory(lib)
add_subdirectory(configure)
# add_subdirectory(test)
add_subdirectory(app)
