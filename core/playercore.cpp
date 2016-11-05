#include "playercore.h"

TPlayerCore::TPlayerCore() :
    mCallback(NULL),
    mBackend(NULL),
    mFront(new TDirectSoundFront()),
    mPlayThread(NULL),
    mPluginManager(new TBackendPluginManager())
{

}

TPlayerCore::~TPlayerCore()
{
    destroyPlayThread();

//    if(mFront)
//        delete mFront;

    if(mBackend)
        delete mBackend;
}

TMusicInfo *TPlayerCore::parse(QString fileName)
{
    TMusicInfo *musicInfo = new TMusicInfo;

    QFileInfo fi(fileName);
    TBackendPlugins plugins = mPluginManager->matchPluginBySuffix(fi.suffix());

    if(plugins.isEmpty())
    {
        TBackendPlugin *plugin = mPluginManager->parse(fileName, musicInfo);
        if(!plugin)
        {
            delete musicInfo;
            return NULL;
        }
    }
    else
    {
        for(auto plugin : plugins)
            if(plugin->parse(fileName, musicInfo))
                return musicInfo;
    }

    delete musicInfo;
    return NULL;
}

void TPlayerCore::setTrack(TTrackInfo *track)
{
    if(!mBackend)
        return;

    mBackend->openTrack(track);
    mFront->setCallback(mBackend->getCallback());
}

void TPlayerCore::setCallback(TPlayCallback callback)
{
    Q_UNUSED(callback)
}

void TPlayerCore::play()
{
    if(!mBackend)
        return;

    if(mPlayThread)
        mPlayThread->resume();
    else
    {
        mPlayThread = new TPlayThread(mFront);
        mPlayThread->start();
    }
}

void TPlayerCore::stop()
{
    if(mPlayThread)
    {
        destroyPlayThread();

        if(mBackend)
            mBackend->closeTrack();
    }
}

void TPlayerCore::pause()
{
    if(mPlayThread)
    {
        mPlayThread->pause();
    }
}

void TPlayerCore::destroyPlayThread()
{
    mPlayThread->needToTerminate();
    mPlayThread->wait();
    delete mPlayThread;
    mPlayThread = NULL;
}

