#ifndef TAPP_H
#define TAPP_H

#include <QApplication>
#include <QSharedMemory>

#include "gui/guimanager.h"
#include "controller/maincontroller.h"
#include "core/core.h"
#include "utils/preferences.h"

#define GLOBAL_SHARE_MEMORY_KEY     "Foreplayer/Theall2"
class TCheckThread : public QThread
{
public:
    explicit TCheckThread(TGuiManager *gui);
    ~TCheckThread();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    TGuiManager *mGui;
    QSharedMemory *mShareMemory;
};

class TApp
{
public:
    explicit TApp(int argc, char *argv[]);
    ~TApp();

    int start();
    static bool isRunning();

private:
    QApplication *mApp;
    TCheckThread *mCheckThread;
};

#endif // TAPP_H
