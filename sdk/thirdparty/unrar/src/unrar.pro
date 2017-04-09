#-------------------------------------------------
#
# Project created by QtCreator 2017-01-22T18:14:33
#
#-------------------------------------------------

QT       -= core gui

TARGET = unrar
TEMPLATE = lib

DEFINES += RARDLL

SOURCES += \
    arccmt.cpp \
    archive.cpp \
    arcread.cpp \
    blake2s.cpp \
    blake2s_sse.cpp \
    blake2sp.cpp \
    cmddata.cpp \
    coder.cpp \
    consio.cpp \
    crc.cpp \
    crypt.cpp \
    crypt1.cpp \
    crypt2.cpp \
    crypt3.cpp \
    crypt5.cpp \
    dll.cpp \
    encname.cpp \
    errhnd.cpp \
    extinfo.cpp \
    extract.cpp \
    filcreat.cpp \
    file.cpp \
    filefn.cpp \
    filestr.cpp \
    find.cpp \
    getbits.cpp \
    global.cpp \
    hardlinks.cpp \
    hash.cpp \
    headers.cpp \
    isnt.cpp \
    list.cpp \
    log.cpp \
    match.cpp \
    model.cpp \
    options.cpp \
    pathfn.cpp \
    qopen.cpp \
    rar.cpp \
    rarpch.cpp \
    rarvm.cpp \
    rarvmtbl.cpp \
    rawread.cpp \
    rdwrfn.cpp \
    recvol.cpp \
    recvol3.cpp \
    recvol5.cpp \
    rijndael.cpp \
    rs.cpp \
    rs16.cpp \
    scantree.cpp \
    secpassword.cpp \
    sha1.cpp \
    sha256.cpp \
    smallfn.cpp \
    strfn.cpp \
    strlist.cpp \
    suballoc.cpp \
    system.cpp \
    threadmisc.cpp \
    threadpool.cpp \
    timefn.cpp \
    ui.cpp \
    uicommon.cpp \
    uiconsole.cpp \
    uisilent.cpp \
    ulinks.cpp \
    unicode.cpp \
    unpack.cpp \
    unpack15.cpp \
    unpack20.cpp \
    unpack30.cpp \
    unpack50.cpp \
    unpack50frag.cpp \
    unpack50mt.cpp \
    unpackinline.cpp \
    uowners.cpp \
    volume.cpp \
    win32acl.cpp \
    win32lnk.cpp \
    win32stm.cpp \
    resource.cpp

HEADERS +=\
        unrar_global.h \
    archive.hpp \
    array.hpp \
    blake2s.hpp \
    cmddata.hpp \
    coder.hpp \
    compress.hpp \
    consio.hpp \
    crc.hpp \
    crypt.hpp \
    dll.hpp \
    dll.rc \
    encname.hpp \
    errhnd.hpp \
    extinfo.hpp \
    extract.hpp \
    filcreat.hpp \
    file.hpp \
    filefn.hpp \
    filestr.hpp \
    find.hpp \
    getbits.hpp \
    global.hpp \
    hash.hpp \
    headers.hpp \
    headers5.hpp \
    isnt.hpp \
    list.hpp \
    loclang.hpp \
    log.hpp \
    match.hpp \
    model.hpp \
    options.hpp \
    os.hpp \
    pathfn.hpp \
    qopen.hpp \
    rar.hpp \
    rardefs.hpp \
    rarlang.hpp \
    raros.hpp \
    rartypes.hpp \
    rarvm.hpp \
    rawread.hpp \
    rdwrfn.hpp \
    recvol.hpp \
    rijndael.hpp \
    rs.hpp \
    rs16.hpp \
    savepos.hpp \
    scantree.hpp \
    secpassword.hpp \
    sha1.hpp \
    sha256.hpp \
    smallfn.hpp \
    strfn.hpp \
    strlist.hpp \
    suballoc.hpp \
    system.hpp \
    threadpool.hpp \
    timefn.hpp \
    ui.hpp \
    ulinks.hpp \
    unicode.hpp \
    unpack.hpp \
    version.hpp \
    volume.hpp \
    resource.hpp

win32 {
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    acknow.txt \
    dll.def \
    license.txt \
    msc.dep \
    readme.txt
