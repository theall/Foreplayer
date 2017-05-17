cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../../)

# List of source files required by libgme and any emulators
# This is not 100% accurate (Fir_Resampler for instance) but
# you'll be OK.
set(libgme_SRCS Blip_Buffer.cpp
                Classic_Emu.cpp
                Data_Reader.cpp
                Dual_Resampler.cpp
                Effects_Buffer.cpp
                Fir_Resampler.cpp
                gme.cpp
                Gme_File.cpp
                M3u_Playlist.cpp
                Multi_Buffer.cpp
                Music_Emu.cpp
                Ay_Apu.cpp
                Ym2612_Emu.cpp
                Sms_Apu.cpp
                Ay_Cpu.cpp
                Ay_Emu.cpp
                Gb_Apu.cpp
                Gb_Cpu.cpp
                Gb_Oscs.cpp
                Gbs_Emu.cpp
                Gym_Emu.cpp
                Hes_Apu.cpp
                Hes_Cpu.cpp
                Hes_Emu.cpp
                Kss_Cpu.cpp
                Kss_Emu.cpp
                Kss_Scc_Apu.cpp
                Nes_Apu.cpp
                Nes_Cpu.cpp
                Nes_Fme7_Apu.cpp
                Nes_Namco_Apu.cpp
                Nes_Oscs.cpp
                Nes_Vrc6_Apu.cpp
                Nsf_Emu.cpp
                Nsfe_Emu.cpp
                Sap_Apu.cpp
                Sap_Cpu.cpp
                Sap_Emu.cpp
                Snes_Spc.cpp
                Spc_Cpu.cpp
                Spc_Dsp.cpp
                Spc_Emu.cpp
                Spc_Filter.cpp
                Vgm_Emu.cpp
                Vgm_Emu_Impl.cpp
                Ym2413_Emu.cpp
                )

# These headers are part of the generic gme interface.
set (EXPORTED_HEADERS gme.h)

# Run during cmake phase, so this is available during make
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gme_types.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/gme_types.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libgme.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libgme.pc @ONLY)

# On some platforms we may need to change headers or whatnot based on whether
# we're building the library or merely using the library. The following is
# only defined when building the library to allow us to tell which is which.
add_definitions(-DBLARGG_BUILD_DLL)

# For the gme_types.h
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Add library to be compiled.
add_library(gme SHARED ${libgme_SRCS})

# The version is the release.  The "soversion" is the API version.  As long
# as only build fixes are performed (i.e. no backwards-incompatible changes
# to the API), the SOVERSION should be the same even when bumping up VERSION.
# The way gme.h is designed, SOVERSION should very rarely be bumped, if ever.
# Hopefully the API can stay compatible with old versions.
#set_target_properties(gme
#    PROPERTIES VERSION ${GME_VERSION}
#               SOVERSION 0)

install(TARGETS gme 
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin  # DLL platforms
    ARCHIVE DESTINATION lib) # DLL platforms

install(FILES ${EXPORTED_HEADERS} DESTINATION include/gme)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libgme.pc DESTINATION lib/pkgconfig)
