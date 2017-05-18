cmake_minimum_required (VERSION 2.6) 
project (Audio3D) 
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../../)
set(CMAKE_MODULE_PATH "${Audio3D_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

SET(FLANN_INCLUDE_DIRS    .)
option(USE_MIT_KEMAR_DATASET  "Use MIT KEMAR HRTF dataset" ON)

find_package(Libsamplerate REQUIRED) 
#find_package(FLANN 1.7.0 REQUIRED)

include_directories(SYSTEM 
                    ${FLANN_INCLUDE_DIRS}
                    ${LIBSAMPLERATE_INCLUDE_DIRS}
                    ${Audio3D_SOURCE_DIR}/kissfft
                    ${Audio3D_SOURCE_DIR}/include 
                    ${Audio3D_SOURCE_DIR}/data 
                   )

add_definitions( -DMIT_KEMAR )

add_library (${PROJECT_NAME} STATIC
    src/audio_3d.cpp 
    src/fft_filter.cpp 
    src/fft_filter_impl.cpp 
    src/hrtf.cpp 
    src/reberation.cpp 
    src/resampler.cpp 
    kissfft/kiss_fft.c 
    kissfft/kiss_fftr.c
) 
target_link_libraries(${PROJECT_NAME} -lsamplerate)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION include/audio3d)
install(TARGETS Audio3D
        ARCHIVE DESTINATION lib)