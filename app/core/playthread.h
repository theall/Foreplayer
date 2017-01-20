#ifndef TPLAYTHREAD_H
#define TPLAYTHREAD_H

#include "pch.h"

#include "front/abstractfront.h"
#include "pluginmanager/backend/backendplugin.h"

class TPlayThread : public QThread
{
    Q_OBJECT

public:
    explicit TPlayThread();
    ~TPlayThread(){ }

    void pause();
    void play();
    void needToTerminate();

    void setBackend(TBackendPlugin *plugin);

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private slots:

private:
    bool mNeedTerminate;

    TAbstractFront *mFront;
    TBackendPlugin *mBackendPlugin;
};
#endif // TPLAYTHREAD_H
