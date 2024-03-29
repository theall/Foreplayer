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
cmake_minimum_required (VERSION 2.6) 
# project name: unzip
project(unzip)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../)

# set source files
set(unzip unzip.c unzip.h)

# set make to a static library
add_library(unzip STATIC ${unzip})

install(FILES ${CMAKE_SOURCE_DIR}/unzip.h DESTINATION include/unzip)
install(TARGETS unzip
        ARCHIVE DESTINATION lib)