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

if(UNIX)
	set(FOREPLAYERLIB_INSTALL_BY_DEFAULT ON)
else(UNIX)
	set(FOREPLAYERLIB_INSTALL_BY_DEFAULT OFF)
endif(UNIX)

option(FOREPLAYERLIB_INSTALL "Install foreplayerlib libs, includes, and CMake scripts" ${FOREPLAYERLIB_INSTALL_BY_DEFAULT})
option(FOREPLAYERLIB_BUILD_SHARED "Build foreplayerlib shared libraries" ON)
option(FOREPLAYERLIB_BUILD_PLUGINS "Build plugins" ON)

set(FOREPLAYERLIB_VERSION 0.1.0)
set(LIB_INSTALL_DIR lib${LIB_SUFFIX})
set(FOREPLAYERLIB_OUTPUT_PATH bin/foreplayer)

if(FOREPLAYERLIB_BUILD_PLUGINS)
    #add_subdirectory(plugins)
endif(FOREPLAYERLIB_BUILD_PLUGINS)

find_package(SDL2 REQUIRED)
#find_package(fftreal REQUIRED)
#find_package(audio3d REQUIRED)

# The foreplayerlib library.
set(source
    src/filter/samplesfilter.cpp
    src/core.cpp
    src/playercore.cpp
    src/playlistcore.cpp
    src/front/sdlfront.cpp
    src/pluginmanager/backend/backendplugin.cpp
    src/front/abstractfront.cpp
    src/pluginmanager/abstractpluginmanager.cpp
    src/pluginmanager/backendpluginmanager.cpp
    src/playthread.cpp
    src/playlist/playlistitem.cpp
    src/playlist/musicitem.cpp
    src/playlist/trackitem.cpp
    src/front/loopbuffer.cpp
)
include_directories(
    src
    thirdparty
    thirdparty/fftreal
    thirdparty/audio3d/include
)
if(FOREPLAYERLIB_BUILD_SHARED)
    add_library(foreplayerlib SHARED ${source})
    target_link_libraries(foreplayerlib ${SDL2_LIBRARY} fftreal audio3d)
else(FOREPLAYERLIB_BUILD_SHARED)
    add_library(foreplayerlib STATIC ${source})
endif(FOREPLAYERLIB_BUILD_SHARED)
