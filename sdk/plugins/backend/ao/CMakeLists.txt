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

# project name: aosdkplugin
project(aosdkplugin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../pluginutils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include/unzip)

set(EXTERNAL_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib)

# set source files
set(aosdkplugin 
    abstractparse.cpp
    fileparse.cpp
    main.cpp
    zipparse.cpp
    ../../pluginutils/pluginutils.cpp
)

# set make to a shared library
add_library(plugin SHARED ${aosdkplugin})
target_link_libraries(plugin -L${EXTERNAL_LIBRARY} -laosdk -lunzip z)

find_library(AOSDK_LIBRARY aosdk HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin)
find_file(AOSDK_LICENSE license.txt HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../thirdparty/aosdk)

if(AOSDK_LIBRARY)
    install(FILES ${AOSDK_LIBRARY} ${AOSDK_LICENSE} DESTINATION ao)
else()
    MESSAGE(WARNING "Didn't find aosdk!")
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
        RUNTIME DESTINATION ao)