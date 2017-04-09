#include "playthread.h"

#include "front/sdlfront.h"

#define CYCLE_INTERVAL      300

TPlayThread::TPlayThread() :
    mNeedTerminate(false),
    mCurrentMicroSeconds(0),
    mState(TS_NULL),
    mFront(NULL),
    mBackendPlugin(NULL),
    mThread(NULL)
{

}

TPlayThread::~TPlayThread()
{
    if(mThread)
    {
        mNeedTerminate = true;
        delete mThread;
        mThread = NULL;
    }
}

void TPlayThread::start()
{
    if(mThread == NULL)
    {
        mThread = new std::thread(&TPlayThread::run, this);
        mThread->detach();
    }
}

void TPlayThread::pause()
{
    if(mFront)
    {
        mFront->pause();
        mState = TS_PAUSED;
    } else {
        mState = TS_READY;
    }
}

void TPlayThread::stop()
{
    if(mFront)
    {
        mFront->stop();
        mState = TS_READY;
    } else {
        mState = TS_READY;
    }
}

void TPlayThread::play()
{
    if(mState != TS_PAUSED)
        mCurrentMicroSeconds = 0;

    if(mFront)
    {
        mFront->play();
        mState = TS_RUNNING;
    } else {
        mState = TS_READY;
    }
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
    if(mFront)
    {
        if(plugin)
        {
            mFront->setSampleSize(plugin->getSampleSize(SAMPLE_RATE, SOUND_FPS));
            // Connect plugin's callback to front's callback
            mFront->setCallback(plugin->getCallback());
        } else {
            mFront->stop();
        }

    }
}

bool TPlayThread::isPaused()
{
    return mState==TS_PAUSED;
}

void TPlayThread::setAudioParameter(TAudioParameter type, float value, int param)
{
    if(mFront)
        mFront->setAudioParameter(type, value, param);
}

void TPlayThread::getAudioData(TAudioDataType dataType, void *param1, void *param2)
{
    if(mFront)
        mFront->getAudioData(dataType, param1, param2);
}

void TPlayThread::run()
{
    if(!mFront)
        mFront = new TSDLFront;

    mFront->start();

    mState = TS_READY;
    while(!mNeedTerminate)
    {
        std::this_thread::sleep_for((std::chrono::milliseconds(CYCLE_INTERVAL)));

        if(mState==TS_RUNNING)
            mCurrentMicroSeconds += CYCLE_INTERVAL;

        // Update progress
        mFront->step();
    }

    mFront->stop();
    mState = TS_TERMINATE;
}
