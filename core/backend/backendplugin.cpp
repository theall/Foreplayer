#include "backendplugin.h"


TBackendPlugin::TBackendPlugin() :
    mLibrary(NULL)
{

}

TBackendPlugin::~TBackendPlugin()
{
    mProcFree();

    delete mLibrary;
}

bool TBackendPlugin::load(QString pluginName)
{
    mLibrary = new QLibrary(pluginName);

    PROC_INITIALIZE initialize = (PROC_INITIALIZE)mLibrary->resolve("initialize");
    if(!initialize)
         return false;

    PROC_MATCHSUFFIXES matchSuffix = (PROC_MATCHSUFFIX)mLibrary->resolve("matchSuffixes");
    if(!matchSuffix)
         return false;

    PROC_PLUGININFO pluginInfo = (PROC_PLUGININFO)mLibrary->resolve("pluginInformation");
    if(!pluginInfo)
        return false;

    mProcParse = (PROC_PARSE)mLibrary->resolve("parse");
    if(!mProcParse)
         return false;

    mProcLoadTrack = (PROC_LOADTRACK)mLibrary->resolve("loadTrack");
    if(!mProcLoadTrack)
         return false;

    mProcCloseTrack = (PROC_CLOSETRACK)mLibrary->resolve("closeTrack");
    if(!mProcCloseTrack)
         return false;

    mProcNextSamples = (TRequestSamples)mLibrary->resolve("nextSamples");
    if(!mProcNextSamples)
         return false;

    mProcFree = (PROC_FREE)mLibrary->resolve("free");
    if(!mProcFree)
         return false;

    // Initialize plugin
    if(!initialize())
        return false;

    // Get match suffixes
    QString matchSuffixes = matchSuffix();
    mMatchSuffixes = matchSuffixes.split(" ");
    if(mMatchSuffixes.isEmpty())
        return false;

    // Plugin information
    pluginInformation(&mPluginInfo);

    mFileName = pluginName;

    return true;
}

bool TBackendPlugin::openTrack(TTrackInfo *track)
{
    mProcLoadTrack(track);
}

bool TBackendPlugin::closeTrack()
{
    mProcCloseTrack();
}

TRequestSamples TBackendPlugin::getCallback()
{
    return mProcNextSamples;
}

bool TBackendPlugin::matchSuffix(QString suffix)
{
    return mMatchSuffixes.contains(suffix, Qt::CaseInsensitive);
}

bool TBackendPlugin::parse(QString file, TMusicInfo *musicInfo)
{
    mProcParse(file.toLocal8Bit().constData(), musicInfo);
}

TPluginInfo *TBackendPlugin::pluginInfo()
{
    return &mPluginInfo;
}

