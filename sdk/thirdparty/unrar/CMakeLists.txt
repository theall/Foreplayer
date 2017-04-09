cmake_minimum_required(VERSION 2.6)

# project name: unrar
project(unrar)

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