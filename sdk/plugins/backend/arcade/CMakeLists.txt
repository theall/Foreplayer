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
cmake_minimum_required(VERSION 2.6)

# project name: m1plugin
project(m1plugin)

include_directories(../../../thirdparty/m1lib/src)
include_directories(../../pluginutils)
include_directories(../)

# set source files
set(m1plugin 
    m1thread.cpp
    main.cpp
    ../../pluginutils/pluginutils.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_library(M1_LIB m1 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)

# set make to a shared library
add_library(plugin SHARED ${m1plugin})
target_link_libraries(plugin ${M1_LIB})

find_library(M1_LIBRARY m1 HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin)
set(M1_DIST ${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/m1lib/dist)
if(M1_LIBRARY)
    install(FILES ${M1_LIBRARY} DESTINATION arcade)
    install(DIRECTORY ${M1_DIST}/lists DESTINATION arcade)
    install(FILES ${M1_DIST}/m1.bin ${M1_DIST}/m1.xml DESTINATION arcade)
else()
    MESSAGE(WARNING "Didn't find m1 library!")
endif()

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
install(TARGETS plugin
        RUNTIME DESTINATION arcade)