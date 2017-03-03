#include "playercore.h"

TPlayerCore::TPlayerCore() :
    mPlayThread(NULL)
  , mPluginManager(TBackendPluginManager::instance())
  , mPlayerState(PS_STOPED)
{
    init();
}

TPlayerCore::~TPlayerCore()
{
    destroyPlayThread();
}

TMusicInfo *TPlayerCore::parse(QString fileName)
{
    TMusicInfo *musicInfo = new TMusicInfo;
    mPluginManager->parse(fileName, musicInfo);
    return musicInfo;
}

bool TPlayerCore::playTrack(TTrackItem *trackItem)
{
    if(!mPluginManager || !mPlayThread)
        return false;

    TTrackInfo trackInfo;
    trackInfo.index = trackItem->index.toInt();
    trackInfo.indexName = trackItem->indexName.toStdString();
    trackInfo.musicFileName = trackItem->fileName.toStdString();

    mPlayerState = PS_STOPED;
    mPlayThread->stop();

    bool result = false;
    // Find a backend plugin which can process this track
    TBackendPlugin *plugin = mPluginManager->loadTrack(&trackInfo);
    if(plugin)
    {
        // Connect plugin's callback to front's callback
        mPlayThread->setAudioParameter(AP_RESET, 0, 0);
        mPlayThread->setBackend(plugin);
        result = true;
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
    }

    return result;
}

void TPlayerCore::setCallback(TPlayCallback callback)
{
    Q_UNUSED(callback)
}

void TPlayerCore::init()
{
    if(mPlayThread)
    {
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
    }
    else
    {
        mPlayThread = new TPlayThread;
        mPlayThread->start();
    }
}

void TPlayerCore::stop()
{
    if(mPlayThread)
        mPlayThread->pause();

    mPlayerState = PS_STOPED;
}

void TPlayerCore::pause()
{
    if(mPlayThread)
    {
        mPlayThread->pause();
        mPlayerState = PS_PAUSED;
    }
}

int TPlayerCore::playedTime()
{
    if(mPlayThread)
    {
        return mPlayThread->playedTime();
    }

    return 0;
}

void TPlayerCore::getAudioData(TAudioDataType dataType, void *param1, void* param2)
{
    if(mPlayThread)
    {
        mPlayThread->getAudioData(dataType, param1, param2);
    }
}

void TPlayerCore::setAudioParameter(TAudioParameter type, float value, int param)
{
    if(mPlayThread)
        mPlayThread->setAudioParameter(type, value, param);
}

bool TPlayerCore::resume()
{
    if(mPlayThread && mPlayThread->isPaused())
    {
        mPlayThread->play();
        mPlayerState = PS_PLAYING;
        return true;
    }
    return false;
}

bool TPlayerCore::isPaused()
{
    return mPlayerState==PS_PAUSED && (mPlayThread && mPlayThread->isPaused());
}

bool TPlayerCore::isStoped()
{
    return mPlayerState==PS_STOPED || !mPlayThread;
}

bool TPlayerCore::isPlaying()
{
    return mPlayerState==PS_PLAYING;
}

void TPlayerCore::destroyPlayThread()
{
    if(mPlayThread)
    {
        stop();
        mPlayThread->needToTerminate();
        mPlayThread->wait();
        delete mPlayThread;
        mPlayThread = NULL;
    }
}

