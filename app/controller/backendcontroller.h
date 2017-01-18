#ifndef TBACKENDCONTROLLER_H
#define TBACKENDCONTROLLER_H

#include "abstractcontroller.h"

class TBackendController : TAbstractController
{
public:
    TBackendController(QObject *parent = 0);

    void joint(TAbstractFront *front);

private:

};

#endif // TBACKENDCONTROLLER_H
