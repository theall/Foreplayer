#ifndef TPLAYTHREAD_H
#define TPLAYTHREAD_H

#include "front/abstractfront.h"
#include "pluginmanager/backend/backendplugin.h"

#include <thread>

enum TThreadState
{
    TS_NULL,
    TS_READY,
    TS_PAUSED,
    TS_RUNNING,
    TS_TERMINATE
};

class TPlayThread
{
public:
    explicit TPlayThread();
    ~TPlayThread();

    void start();

    void pause();
    void stop();
    void play();
    void needToTerminate();

    int playedTime();

    void setBackend(TBackendPlugin *plugin);

    bool isPaused();

    void setAudioParameter(TAudioParameter type, float value, int param);
    void getAudioData(TAudioDataType dataType, void *param1, void* param2);

protected:
    void run();

private slots:

private:
    bool mNeedTerminate;
    int mCurrentMicroSeconds;
    TThreadState mState;

    TAbstractFront *mFront;
    TBackendPlugin *mBackendPlugin;

    std::thread *mThread;
};
#endif // TPLAYTHREAD_H
