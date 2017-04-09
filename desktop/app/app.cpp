#include "app.h"

#include "version/version.h"
#include "utils/preferences.h"

#include <QTextCodec>

TCheckThread::TCheckThread(TGuiManager *gui) :
    QThread()
  , mGui(gui)
  , mShareMemory(new QSharedMemory(GLOBAL_SHARE_MEMORY_KEY))
{
#ifndef QT_DEBUG
    mShareMemory->create(1);
    mShareMemory->attach();
#endif
}

TCheckThread::~TCheckThread()
{
    if(mShareMemory)
    {
        delete mShareMemory;
        mShareMemory = NULL;
    }
}

void TCheckThread::run()
{
    while(true)
    {
        mShareMemory->lock();
        char *b = (char*)mShareMemory->data();
        if(b && *b && mGui)
        {
            *b = 0;
            mGui->show();
        }
        mShareMemory->unlock();
        msleep(500);
    }
}

TApp::TApp(int argc, char *argv[]) :
    mApp(new QApplication(argc, argv))
  , mCheckThread(NULL)
{
    TVersionInfo *vi = TVersionInfo::instance(QString(argv[0]));
    mApp->setOrganizationDomain(vi->domain());
    mApp->setApplicationName(vi->productName());
    mApp->setApplicationVersion(vi->productVersion());

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    TPreferences::instance();
}

TApp::~TApp()
{
    TPreferences::deleteInstance();

    if(mCheckThread)
    {
        mCheckThread->terminate();
        delete mCheckThread;
        mCheckThread = NULL;
    }
}

int TApp::start()
{
    TMainController controller;
    TGuiManager gui(&controller);
    TCore *core;

    try
    {
        core = TCore::instance();
    } catch(QString s) {
        gui.mainWindow()->setTitles(s);
    }

    if(!controller.joint(&gui, core))
        return 0;

#ifndef QT_DEBUG
    if(!mCheckThread)
    {
        mCheckThread = new TCheckThread(&gui);
        mCheckThread->start();
    }
#endif

    return mApp->exec();
}

bool TApp::isRunning()
{
#ifndef QT_DEBUG
    static QSharedMemory data(GLOBAL_SHARE_MEMORY_KEY);
    if(!data.create(1))
    {
        data.attach();
        data.lock();
        *(char*)data.data() = 1;
        data.unlock();
        data.detach();
        return true;
    }
#endif
    return false;
}
