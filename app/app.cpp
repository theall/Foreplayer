#include "app.h"

TApp::TApp(int argc, char *argv[]) :
    mApp(new QApplication(argc, argv))
{
    mApp->setOrganizationDomain("Theall");
    mApp->setApplicationName("Foreplayer");
    mApp->setApplicationVersion("0.0.1");

    TPreferences::instance();
}

TApp::~TApp()
{
    TPreferences::deleteInstance();
}

int TApp::start()
{
    TCore core;
    TMainController controller;
    TGuiManager gui(&controller);
    controller.joint(&gui, &core);

    return mApp->exec();
}

