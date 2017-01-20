#include "playthread.h"

#include "front/sdlfront.h"
TPlayThread::TPlayThread():
    mNeedTerminate(false),
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
}

void TPlayThread::needToTerminate()
{
    mNeedTerminate = true;
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

void TPlayThread::run()
{
    if(!mFront)
        mFront = new TSDLFront;

    mFront->start();

    while(!mNeedTerminate)
    {
        msleep(100);

        mFront->step();
    }

    mFront->stop();
}
