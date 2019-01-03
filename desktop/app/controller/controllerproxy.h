#ifndef CONTROLLERADAPTER_H
#define CONTROLLERADAPTER_H

#include <QObject>
#include "../core/core.h"
#include "../gui/guiproxy.h"
#include "legacy/maincontroller.h"

class TControllerProxy : public QObject
{
    Q_OBJECT

public:
    explicit TControllerProxy(QObject *parent = 0);

    bool joint(TGuiProxy *guiProxy, TCore *core);

private:
    TMainController *mLegacyController;
};

#endif // CONTROLLERADAPTER_H
