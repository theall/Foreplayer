#include "controllerproxy.h"

TControllerProxy::TControllerProxy(QObject *parent)
{
    mLegacyController = new TMainController(this);
}

bool TControllerProxy::joint(TGuiProxy *guiProxy, TCore *core)
{
    mLegacyController->joint(guiProxy->getGuiManager(), core);
}
