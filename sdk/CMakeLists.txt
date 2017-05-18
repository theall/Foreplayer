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

cmake_minimum_required(VERSION 3.0)

project(foreplayerlib)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CUSTOME_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_definitions(-DFOREPLAYER_EXPORT)
add_compile_options(-std=c++11)

if(UNIX)
	set(FOREPLAYERLIB_INSTALL_BY_DEFAULT ON)
else(UNIX)
	set(FOREPLAYERLIB_INSTALL_BY_DEFAULT OFF)
endif(UNIX)

option(FOREPLAYERLIB_INSTALL "Install foreplayerlib libs, includes, and CMake scripts" ${FOREPLAYERLIB_INSTALL_BY_DEFAULT})
option(FOREPLAYERLIB_BUILD_SHARED "Build foreplayerlib shared libraries" ON)

set(FOREPLAYERLIB_VERSION 0.1.0)
set(LIB_INSTALL_DIR lib${LIB_SUFFIX})
set(FOREPLAYERLIB_OUTPUT_PATH bin/foreplayer)

find_package(SDL2 REQUIRED)

# The foreplayerlib library.
set(source
    src/core.cpp
    src/cplusutils.cpp
    src/exportcore.cpp
    src/foreplayerlib.cpp
    src/playercore.cpp
    src/playlistcore.cpp
    src/playthread.cpp
    src/strformat.hpp
    src/filter/samplesfilter.cpp
    src/front/abstractfront.cpp
    src/front/loopbuffer.cpp
    src/front/sdlfront.cpp
    src/playlist/abstractitem.cpp
    src/playlist/musicitem.cpp
    src/playlist/playlistitem.cpp
    src/playlist/trackitem.cpp
    src/pluginmanager/abstractpluginmanager.cpp
    src/pluginmanager/backendpluginmanager.cpp
    src/pluginmanager/library.cpp
    src/pluginmanager/backend/backendplugin.cpp
)

include_directories(
    src
    include
    thirdparty
    ${SDL2_INCLUDE_DIR}/../
)

if(FOREPLAYERLIB_BUILD_SHARED)
    add_library(foreplayerlib SHARED ${source})
    target_link_libraries(foreplayerlib -LSDL2_LIBRARY -lSDL2 -L${CMAKE_CUSTOME_LIB_PATH} -lfftreal -laudio3d -lsamplerate -lz)
else(FOREPLAYERLIB_BUILD_SHARED)
    add_library(foreplayerlib STATIC ${source})
endif(FOREPLAYERLIB_BUILD_SHARED)

#install(FILES ${CMAKE_SOURCE_DIR}/include/foreplayer.h DESTINATION include)
install(TARGETS foreplayerlib
        RUNTIME DESTINATION ../dist/sdk
        ARCHIVE DESTINATION lib)
        