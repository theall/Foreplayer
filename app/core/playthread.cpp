#include "playthread.h"

TPlayThread::TPlayThread(TAbstractFront *front):
    mPaused(true),
    mNeedTerminate(false),
    mFront(front)
{

}

void TPlayThread::pause()
{
    mPaused = true;
}

void TPlayThread::resume()
{

}

void TPlayThread::needToTerminate()
{
    mNeedTerminate = true;
}

void TPlayThread::run()
{
    if(!mFront)
        return;

    mFront->start();

    while(!mNeedTerminate)
    {
        msleep(100);

        if(mPaused)
            continue;

        mFront->step();
    }

    mFront->stop();
}
