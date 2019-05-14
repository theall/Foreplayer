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

# project name: mp3lame
project(mp3lame)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_definitions(-DHAVE_CONFIG_H)

include_directories(include)
include_directories(libmp3lame)

IF (WIN32)
    file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/configMS.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)
    file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/include/configMS.h ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)
ELSEIF (APPLE)

ELSEIF (UNIX)

ENDIF ()

#add_compile_options(-fdata-sections)
#add_compile_options(-ffunction-sections)

# set source files
set(src 
    libmp3lame/bitstream.c
    libmp3lame/encoder.c
    libmp3lame/fft.c
    libmp3lame/gain_analysis.c
    libmp3lame/id3tag.c
    libmp3lame/lame.c
    libmp3lame/mpglib_interface.c
    libmp3lame/newmdct.c
    libmp3lame/presets.c
    libmp3lame/psymodel.c
    libmp3lame/quantize.c
    libmp3lame/quantize_pvt.c
    libmp3lame/reservoir.c
    libmp3lame/set_get.c
    libmp3lame/tables.c
    libmp3lame/takehiro.c
    libmp3lame/util.c
    libmp3lame/vbrquantize.c
    libmp3lame/VbrTag.c
    libmp3lame/version.c
    libmp3lame/vector/xmm_quantize_sub.c
)

# set make to a SHARED library
add_library(mp3lame SHARED ${src})
target_link_libraries(mp3lame)

install(FILES ${CMAKE_SOURCE_DIR}/include/lame.h DESTINATION include/lame)
install(TARGETS mp3lame
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
        
        
