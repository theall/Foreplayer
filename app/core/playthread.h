#ifndef TPLAYTHREAD_H
#define TPLAYTHREAD_H

#include "pch.h"

#include "front/abstractfront.h"
#include "pluginmanager/backend/backendplugin.h"

enum TThreadState
{
    TS_NULL,
    TS_READY,
    TS_PAUSED,
    TS_RUNNING,
    TS_TERMINATE
};

class TPlayThread : public QThread
{
    Q_OBJECT

public:
    explicit TPlayThread();
    ~TPlayThread(){ }

    void pause();
    void stop();
    void play();
    void needToTerminate();

    int playedTime();

    void setBackend(TBackendPlugin *plugin);
    void currentSamples(int *size, short **samples);

    bool isPaused();

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private slots:

private:
    bool mNeedTerminate;
    int mCurrentMicroSeconds;
    TThreadState mState;

    TAbstractFront *mFront;
    TBackendPlugin *mBackendPlugin;
};
#endif // TPLAYTHREAD_H
