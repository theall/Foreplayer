#ifndef TAPP_H
#define TAPP_H

#include <QApplication>
#include <QSharedMemory>

#include "controller/maincontroller.h"

#define GLOBAL_SHARE_MEMORY_KEY     "Foreplayer/Theall"
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
