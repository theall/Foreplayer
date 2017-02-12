#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T09:31:26
#
#----------------------------------------------- --
CONFIG += c++11

TARGET = foreplayer
TEMPLATE = app

SOURCES += main.cpp

include(app/app.pri)

CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
}
