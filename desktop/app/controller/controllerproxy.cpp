#include "controllerproxy.h"

TControllerProxy::TControllerProxy(QObject *parent)
{
    mLegacyController = new TMainController(parent);
}

bool TControllerProxy::joint(TGuiProxy *guiProxy, TCore *core)
{
    return mLegacyController->joint(guiProxy->getGuiManager(), core);
}
