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

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

# set source files
set(unrar 
    src/archive.cpp
    src/arcread.cpp
    src/blake2s.cpp
    src/cmddata.cpp
    src/consio.cpp
    src/crc.cpp
    src/crypt.cpp
    src/dll.cpp
    src/encname.cpp
    src/errhnd.cpp
    src/extinfo.cpp
    src/extract.cpp
    src/filcreat.cpp
    src/file.cpp
    src/filefn.cpp
    src/filestr.cpp
    src/find.cpp
    src/getbits.cpp
    src/global.cpp
    src/hash.cpp
    src/headers.cpp
    src/isnt.cpp
    src/list.cpp
    src/match.cpp
    src/options.cpp
    src/pathfn.cpp
    src/qopen.cpp
    src/rar.cpp
    src/rarpch.cpp
    src/rarvm.cpp
    src/rawread.cpp
    src/rdwrfn.cpp
    src/recvol.cpp
    src/rijndael.cpp
    src/rs.cpp
    src/rs16.cpp
    src/scantree.cpp
    src/secpassword.cpp
    src/sha1.cpp
    src/sha256.cpp
    src/smallfn.cpp
    src/strfn.cpp
    src/strlist.cpp
    src/system.cpp
    src/threadpool.cpp
    src/timefn.cpp
    src/ui.cpp
    src/unicode.cpp
    src/unpack.cpp
    src/volume.cpp
    src/resource.cpp
)

# set make to a shared library
add_library(unrar SHARED ${unrar})

install(FILES ${CMAKE_SOURCE_DIR}/include/unrar.h DESTINATION include)
install(TARGETS unrar
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib)
        