#include <QApplication>

#include "gui/guimanager.h"
#include "controller/maincontroller.h"
#include "core/core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TCore core;

    TMainController controller;

    TGuiManager gui(&controller);

    gui.loadSkin("Z:/skins/fulkfour/skin.xml");

    controller.joint(&gui, &core);

    a.exec();
    return 0;
}
