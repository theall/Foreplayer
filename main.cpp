#include <QApplication>

#include "gui/tguimanager.h"
#include "controller/controller.h"
#include "core/core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TCore core;

    TController controller;

    TGuiManager gui(&controller);

    gui.loadSkin("Z:/skins/orange/skin.xml");

    controller.joint(&gui, &core);

    a.exec();
    return 0;
}
