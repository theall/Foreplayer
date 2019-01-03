#include "guiproxy.h"

TGuiProxy::TGuiProxy(QObject *parent) :
    mGuiManager(NULL)
{
    mGuiManager = new TGuiManager(this);
}

void TGuiProxy::exit()
{
    if(mGuiManager)
        mGuiManager->exit();
}

void TGuiProxy::open()
{
    if(mGuiManager)
        mGuiManager->open();
}

void TGuiProxy::restoreGui()
{
    if(mGuiManager)
        mGuiManager->restoreGui();
}

void TGuiProxy::setTitle(QString title)
{
    if(mGuiManager)
        mGuiManager->mainWindow()->setTitle(title);
}

TGuiManager *TGuiProxy::getGuiManager()
{
    return mGuiManager;
}
