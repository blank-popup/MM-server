cmake_minimum_required(VERSION 3.8)

set(name_target testPython)
build_plugin_so(${name_target} ${name_target}.c ${name_target}Ext.c ${name_target}Task.c)

set(name_file testPython.py)
copy_plugin_py(${name_file})
