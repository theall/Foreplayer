#
# Copyright (c) Bilge Theall
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

cmake_minimum_required(VERSION 3.6)

# project name: gmeplugin
project(gmeplugin)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../../../dist/plugins/backend)

include_directories(../)
include_directories(../../pluginutils)
include_directories(../../../thirdparty/gme/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

# set source files
set(gmeplugin 
    abstractparse.cpp
    fileparse.cpp
    gmewrap.cpp
    main.cpp
    rarparse.cpp
    zipparse.cpp
    ../../pluginutils/pluginutils.cpp
)

find_library(GME_LIB gme HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)
find_library(UNRAR_LIB unrar HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)
find_library(UNZIP_LIB unzip HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)

# set make to a shared library
add_library(plugin SHARED ${gmeplugin})
target_link_libraries(plugin ${GME_LIB} ${UNRAR_LIB} ${UNZIP_LIB} z)

find_library(GME_LIBRARY gme HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin)
if(GME_LIBRARY)
    install(FILES ${GME_LIBRARY} DESTINATION gme)
else()
    MESSAGE(WARNING "Didn't find gme library!")
endif()

find_library(UNRAR_LIBRARY unrar HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin)
if(UNRAR_LIBRARY)
    install(FILES ${UNRAR_LIBRARY} DESTINATION gme)
else()
    MESSAGE(WARNING "Didn't find unrar library!")
endif()

install(TARGETS plugin
        RUNTIME DESTINATION gme)
        