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

# project name: aosdk
project(aosdk)

add_definitions(-DLSB_FIRST=1 -DLSB_FIRST -DPATH_MAX=1024 -DHAS_PSXCPU)

include_directories(.)
include_directories(eng_ssf)
include_directories(eng_qsf)
include_directories(eng_dsf)

#add_compile_options(-std=c++11)
add_compile_options(-fdata-sections)
add_compile_options(-ffunction-sections)

# set source files
set(src 
    eng_dsf/aica.c
    eng_dsf/aicadsp.c
    eng_dsf/arm7.c
    eng_dsf/arm7i.c
    eng_dsf/dc_hw.c
    eng_dsf/eng_dsf.c
    eng_psf/eng_psf.c
    eng_psf/eng_psf2.c
    eng_psf/eng_spu.c
    eng_psf/psx.c
    eng_psf/psx_hw.c
    eng_qsf/eng_qsf.c
    eng_qsf/kabuki.c
    eng_qsf/qsound.c
    eng_qsf/z80.c
    eng_qsf/z80dasm.c
    eng_ssf/eng_ssf.c
    eng_ssf/m68kcpu.c
    eng_ssf/m68kopac.c
    eng_ssf/m68kopdm.c
    eng_ssf/m68kopnz.c
    eng_ssf/m68kops.c
    eng_ssf/sat_hw.c
    eng_ssf/scsp.c
    eng_ssf/scspdsp.c
    eng_psf/peops/spu.c
    eng_psf/peops2/dma.c
    eng_psf/peops2/registers.c
    eng_psf/peops2/spu.c
    aosdk.c
    ao.c
    corlett.c
    m1sdr.c
    utils.c
    mem.c
)

find_package(sdl2)
find_package(zlib)

# set make to a SHARED library
add_library(aosdk SHARED ${src})
target_link_libraries(aosdk z sdl2)
