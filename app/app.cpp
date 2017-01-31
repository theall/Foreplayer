#include "app.h"

#include <QTextCodec>
#include <QSharedMemory>

TApp::TApp(int argc, char *argv[]) :
    mApp(new QApplication(argc, argv))
{
    mApp->setOrganizationDomain("Theall");
    mApp->setApplicationName("Foreplayer");
    mApp->setApplicationVersion("0.0.1");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

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

bool TApp::isRunning()
{
    static QSharedMemory data("Foreplayer/Theall");
    if(data.create(100) == false)
    {
        return true;
    }
    return false;
}
