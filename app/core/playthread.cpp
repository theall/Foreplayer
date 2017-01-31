#include "playthread.h"

#include "front/sdlfront.h"

#define CYCLE_INTERVAL      300

TPlayThread::TPlayThread():
    mNeedTerminate(false),
    mCurrentMicroSeconds(0),
    mFront(NULL),
    mBackendPlugin(NULL)
{

}

void TPlayThread::pause()
{
    if(mFront)
        mFront->pause();
}

void TPlayThread::play()
{
    if(mFront)
        mFront->play();

    mCurrentMicroSeconds = 0;
}

void TPlayThread::needToTerminate()
{
    mNeedTerminate = true;
}

int TPlayThread::playedTime()
{
    return mCurrentMicroSeconds;
}

void TPlayThread::setBackend(TBackendPlugin *plugin)
{
    if(mBackendPlugin == plugin)
        return;

    if(mBackendPlugin)
        mBackendPlugin->closeTrack();

    mBackendPlugin = plugin;
    if(mFront && plugin)
    {
        // Connect plugin's callback to front's callback
        mFront->setCallback(plugin->getCallback());
    }
}

void TPlayThread::currentSamples(int *size, short **samples)
{
    if(mFront)
        mFront->currentSamples(size, samples);
    else {
        *size = 0;
        *samples = NULL;
    }
}

void TPlayThread::run()
{
    if(!mFront)
        mFront = new TSDLFront;

    mFront->start();

    while(!mNeedTerminate)
    {
        msleep(CYCLE_INTERVAL);

        mCurrentMicroSeconds += CYCLE_INTERVAL;

        // Update progress
        mFront->step();
    }

    mFront->stop();
}
