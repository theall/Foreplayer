QT += core
QT -= gui

TARGET = testAoSDK
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11 object_parallel_to_source
TEMPLATE = app

DEFINES += NO_GUI LSB_FIRST=1 HAS_PSXCPU=1 PATH_MAX=1024

LIBS += -lm -lz
LIBS += -lcomdlg32 -lgdi32 -lole32 -lpsapi -lshlwapi -lversion -lwininet
LIBS += -ldsound -ldxguid
# Windows DLLs referenced by win32_utf8, which are hopefully eliminated by
# -gc-sections one day…

INCLUDEPATH += \
    ../

SOURCES += \
    main.c \
    mididump.c \
    sampledump.c \
    wavedump.c \
    argparse/argparse.c \
    ../eng_dsf/aica.c \
    ../eng_dsf/aicadsp.c \
    ../eng_dsf/aicalfo.c \
    ../eng_dsf/arm7.c \
    ../eng_dsf/arm7i.c \
    ../eng_dsf/arm7memil.c \
    ../eng_dsf/dc_hw.c \
    ../eng_dsf/eng_dsf.c \
    ../eng_psf/peops/adsr.c \
    ../eng_psf/peops/dma.c \
    ../eng_psf/peops/registers.c \
    ../eng_psf/peops/reverb.c \
    ../eng_psf/peops/spu.c \
    ../eng_psf/peops2/adsr.c \
    ../eng_psf/peops2/dma.c \
    ../eng_psf/peops2/registers.c \
    ../eng_psf/peops2/reverb.c \
    ../eng_psf/peops2/spu.c \
    ../eng_psf/peops2/xa.c \
    ../eng_psf/eng_psf.c \
    ../eng_psf/eng_psf2.c \
    ../eng_psf/eng_spu.c \
    ../eng_psf/psx.c \
    ../eng_psf/psx_hw.c \
    ../eng_qsf/eng_qsf.c \
    ../eng_qsf/kabuki.c \
    ../eng_qsf/qsound.c \
    ../eng_qsf/z80.c \
    ../eng_qsf/z80dasm.c \
    ../eng_ssf/eng_ssf.c \
    ../eng_ssf/m68kcpu.c \
    ../eng_ssf/m68kopac.c \
    ../eng_ssf/m68kopdm.c \
    ../eng_ssf/m68kopnz.c \
    ../eng_ssf/m68kops.c \
    ../eng_ssf/sat_hw.c \
    ../eng_ssf/scsp.c \
    ../eng_ssf/scspdsp.c \
    ../eng_ssf/scsplfo.c \
    ../ao.c \
    ../corlett.c \
    ../dsnd.c \
    ../m1sdr.c \
    ../mem.c \
    ../utils.c

HEADERS += \
    mididump.h \
    sampledump.h \
    wavedump.h \
    argparse/argparse.h \
    ../eng_dsf/aica.h \
    ../eng_dsf/arm7.h \
    ../eng_dsf/arm7i.h \
    ../eng_dsf/arm7thumb.h \
    ../eng_dsf/dc_hw.h \
    ../eng_psf/peops/adsr.h \
    ../eng_psf/peops/dma.h \
    ../eng_psf/peops/externals.h \
    ../eng_psf/peops/gauss_i.h \
    ../eng_psf/peops/registers.h \
    ../eng_psf/peops/regs.h \
    ../eng_psf/peops/spu.h \
    ../eng_psf/peops/stdafx.h \
    ../eng_psf/peops2/adsr.h \
    ../eng_psf/peops2/dma.h \
    ../eng_psf/peops2/externals.h \
    ../eng_psf/peops2/gauss_i.h \
    ../eng_psf/peops2/psemuxa.h \
    ../eng_psf/peops2/registers.h \
    ../eng_psf/peops2/regs.h \
    ../eng_psf/peops2/reverb.h \
    ../eng_psf/peops2/spu.h \
    ../eng_psf/peops2/stdafx.h \
    ../eng_psf/cpuintrf.h \
    ../eng_psf/mamemem.h \
    ../eng_psf/psx.h \
    ../eng_qsf/qsound.h \
    ../eng_qsf/z80.h \
    ../eng_qsf/z80dasm.h \
    ../eng_ssf/m68k.h \
    ../eng_ssf/m68kconf.h \
    ../eng_ssf/m68kcpu.h \
    ../eng_ssf/m68kmame.h \
    ../eng_ssf/m68kops.h \
    ../eng_ssf/sat_hw.h \
    ../eng_ssf/scsp.h

