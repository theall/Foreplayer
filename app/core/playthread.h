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

    int playedTime();

    void setBackend(TBackendPlugin *plugin);
    void currentSamples(int *size, short **samples);

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private slots:

private:
    bool mNeedTerminate;
    int mCurrentMicroSeconds;

    TAbstractFront *mFront;
    TBackendPlugin *mBackendPlugin;
};
#endif // TPLAYTHREAD_H
