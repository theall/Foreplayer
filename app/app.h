#ifndef TAPP_H
#define TAPP_H

#include <QApplication>

#include "gui/guimanager.h"
#include "controller/maincontroller.h"
#include "core/core.h"
#include "utils/preferences.h"

class TApp
{
public:
    explicit TApp(int argc, char *argv[]);
    ~TApp();

    int start();
    static bool isRunning();

private:
    QApplication *mApp;
};

#endif // TAPP_H
