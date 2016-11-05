#include "backendcontroller.h"

TBackendController::TBackendController(QObject *parent) :
    TAbstractController(parent),
    mBackendManager(new TBackendPluginManager(this))
{

}

