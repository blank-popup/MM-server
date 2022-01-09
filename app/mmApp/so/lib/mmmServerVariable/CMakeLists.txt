cmake_minimum_required(VERSION 3.8)

set(name_target mmmServerVariable)
build_lib_so(${name_target} ${name_target}.c ${name_target}Ext.c ${name_target}Task.c)
