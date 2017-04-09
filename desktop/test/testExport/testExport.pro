QT += core gui widgets xml

TARGET = testExport
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG(debug, debug|release){
    DESTDIR = z:/build/foreplayer/debug/debug
}
