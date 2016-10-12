#include "backendcontroller.h"

TBackendController::TBackendController(QObject *parent) :
    QObject(parent),
    mBackendManager(new TBackendPluginManager(this))
{

}

