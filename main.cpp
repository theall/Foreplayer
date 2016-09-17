#include <QApplication>

#include "tguimanager.h"
#include "skinloader.h"
#include "timagebutton.h"
#include "tvisualwidget.h"
#include "tcontroller.h"
#include "tplayercore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TPlayerCore core;

    TController controller;

    TGuiManager gui(&controller);

    gui.loadSkin("Z:/skins/orange/skin.xml");

    controller.joint(&gui, &core);

    a.exec();
    return 0;
}
