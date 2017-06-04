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

# project name: bassplugin
project(bassplugin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../pluginutils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/bass/include)

# set source files
set(bassplugin 
    main.cpp
    ../../pluginutils/pluginutils.cpp
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")  
    set(DIST_PATH linux)  
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")  
    set(DIST_PATH win32)
elseif (CMAKE_SYSTEM_NAME MATCHES "mac")
    set(DIST_PATH mac)
elseif (CMAKE_SYSTEM_NAME MATCHES "android")
    set(DIST_PATH android) 
elseif (CMAKE_SYSTEM_NAME MATCHES "ios")
    set(DIST_PATH ios) 
else()
    MESSAGE(ERROR "Unsupported platform!")
endif()
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../../../dist/${DIST_PATH}/plugins/backend)

find_library(BASS_LIBRARY bass HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/bass/bin)
find_file(BASS_LICENSE license.txt HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/bass/dist)

# set make to a shared library
add_library(plugin SHARED ${bassplugin})
if(UNIX)
    target_link_libraries(plugin bass)
else(UNIX)
    target_link_libraries(plugin ${BASS_LIBRARY})
endif(UNIX)

if(BASS_LIBRARY)
    install(FILES ${BASS_LIBRARY} ${BASS_LICENSE} DESTINATION default)
else()
    MESSAGE(WARNING "Didn't find bass library!")
endif()

install(TARGETS plugin
        RUNTIME DESTINATION default)
        