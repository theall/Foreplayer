#include "playercore.h"

TPlayerCore::TPlayerCore() :
    mPlayThread(NULL),
    mPluginManager(TBackendPluginManager::instance())
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
    TBackendPlugin *plugin = mPluginManager->parse(fileName, musicInfo);
    return musicInfo;
}

bool TPlayerCore::playTrack(TTrackItem *trackItem)
{
    if(!mPluginManager || !mPlayThread)
        return false;

    TTrackInfo trackInfo;
    trackInfo.index = trackItem->index.toInt();
    trackInfo.indexName = trackItem->indexName.toStdString();
    trackInfo.musicFileName = trackItem->musicFilePath->toStdString();

    mPlayThread->pause();

    bool result = false;
    // Find a backend plugin which can process this track
    TBackendPlugin *plugin = mPluginManager->loadTrack(&trackInfo);
    if(plugin)
    {
        // Connect plugin's callback to front's callback
        mPlayThread->setBackend(plugin);
        result = true;
    }
    mPlayThread->play();

    return result;
}

void TPlayerCore::setCallback(TPlayCallback callback)
{
    Q_UNUSED(callback)
}

void TPlayerCore::init()
{
    if(mPlayThread)
        mPlayThread->play();
    else
    {
        mPlayThread = new TPlayThread;
        mPlayThread->start();
    }
}

void TPlayerCore::stop()
{

    destroyPlayThread();
}

void TPlayerCore::pause()
{
    if(mPlayThread)
    {
        mPlayThread->pause();
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

void TPlayerCore::currentSamples(int *size, short **samples)
{
    if(mPlayThread)
    {
        return mPlayThread->currentSamples(size, samples);
    }
}

void TPlayerCore::destroyPlayThread()
{
    if(mPlayThread)
    {
        mPlayThread->needToTerminate();
        mPlayThread->wait();
        delete mPlayThread;
        mPlayThread = NULL;
    }
}

