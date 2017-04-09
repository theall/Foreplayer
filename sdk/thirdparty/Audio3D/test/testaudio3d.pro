QT += core
QT -= gui

TARGET = testaudio3d
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -lsdl2
LIBS += -lsamplerate

DEFINES += MIT_KEMAR

INCLUDEPATH += ../include \
    ../data \
    ../kissfft \
    ../

HEADERS += \
    ../include/audio_3d.h \
    ../include/fft_filter.h \
    ../include/fft_filter_impl.h \
    ../include/flann_nn_search.hpp \
    ../include/hrtf.h \
    ../include/hrtf_data.h \
    ../include/reberation.h \
    ../include/resampler.h \
    ../kissfft/_kiss_fft_guts.h \
    ../kissfft/kiss_fft.h \
    ../kissfft/kiss_fftr.h

SOURCES += pa_sample.cpp \
    ../src/audio_3d.cpp \
    ../src/fft_filter.cpp \
    ../src/fft_filter_impl.cpp \
    ../src/hrtf.cpp \
    ../src/reberation.cpp \
    ../src/resampler.cpp \
    ../kissfft/kiss_fft.c \
    ../kissfft/kiss_fftr.c
