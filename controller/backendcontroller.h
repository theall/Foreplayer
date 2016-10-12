#ifndef TBACKENDCONTROLLER_H
#define TBACKENDCONTROLLER_H

#include "pch.h"

#include "../core/front/abstractfront.h"
#include "pluginmanager/backendpluginmanager.h"

class TBackendController : QObject
{
public:
    TBackendController(QObject *parent = 0);

    void joint(TAbstractFront *front);

private:
    TBackendPluginManager *mBackendManager;
};

#endif // TBACKENDCONTROLLER_H
