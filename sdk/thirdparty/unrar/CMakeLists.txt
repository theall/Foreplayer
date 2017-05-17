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

# project name: unrar
project(unrar)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_definitions(-DRARDLL)

# set source files
set(unrar 
    src/rs16.cpp
    src/filefn.cpp
    src/rar.cpp
    src/filestr.cpp
    src/volume.cpp
    src/list.cpp
    src/hash.cpp
    src/system.cpp
    src/errhnd.cpp
    src/rs.cpp
    src/cmddata.cpp
    src/match.cpp
    src/timefn.cpp
    src/filcreat.cpp
    src/rdwrfn.cpp
    src/dll.cpp
    src/pathfn.cpp
    src/sha1.cpp
    src/strlist.cpp
    src/extract.cpp
    src/rarpch.cpp
    src/crc.cpp
    src/file.cpp
    src/getbits.cpp
    src/strfn.cpp
    src/global.cpp
    src/rijndael.cpp
    src/options.cpp
    src/qopen.cpp
    src/rawread.cpp
    src/scantree.cpp
    src/find.cpp
    src/resource.cpp
    src/smallfn.cpp
    src/headers.cpp
    src/sha256.cpp
    src/arcread.cpp
    src/encname.cpp
    src/secpassword.cpp
    src/isnt.cpp
    src/unicode.cpp
)

# set make to a static library
add_library(unrar STATIC ${unrar})

install(FILES ${CMAKE_SOURCE_DIR}/include/unrar.h DESTINATION include)
install(TARGETS unrar
        ARCHIVE DESTINATION lib)
        