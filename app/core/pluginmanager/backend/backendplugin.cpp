#include "backendplugin.h"


TBackendPlugin::TBackendPlugin() :
    mLibrary(NULL),
    mProcParse(NULL),
    mProcLoadTrack(NULL),
    mProcCloseTrack(NULL),
    mProcNextSamples(NULL),
    mProcFree(NULL)
{

}

TBackendPlugin::~TBackendPlugin()
{
    if(mProcFree)
    {
        mProcFree();
        mProcFree = NULL;
    }
    if(mLibrary)
    {
        delete mLibrary;
        mLibrary = NULL;
    }
}

bool TBackendPlugin::load(QString pluginName)
{
    mLibrary = new QLibrary(pluginName);

    PROC_INITIALIZE initialize = (PROC_INITIALIZE)mLibrary->resolve("initialize");
    if(!initialize)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("initialize").toLatin1().constData();
        return false;
    }
    PROC_MATCHSUFFIXES matchSuffix = (PROC_MATCHSUFFIXES)mLibrary->resolve("matchSuffixes");
    if(!matchSuffix)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("matchSuffixes").toLatin1().constData();
        return false;
    }
    PROC_PLUGININFO pluginInfo = (PROC_PLUGININFO)mLibrary->resolve("pluginInformation");
    if(!pluginInfo)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("pluginInformation").toLatin1().constData();
        return false;
    }
    mProcParse = (PROC_PARSE)mLibrary->resolve("parse");
    if(!mProcParse)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("parse").toLatin1().constData();
        return false;
    }
    mProcLoadTrack = (PROC_LOADTRACK)mLibrary->resolve("loadTrack");
    if(!mProcLoadTrack)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("loadTrack").toLatin1().constData();
        return false;
    }
    mProcCloseTrack = (PROC_CLOSETRACK)mLibrary->resolve("closeTrack");
    if(!mProcCloseTrack)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("closeTrack").toLatin1().constData();
        return false;
    }
    mProcNextSamples = (TRequestSamples)mLibrary->resolve("nextSamples");
    if(!mProcNextSamples)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("nextSamples").toLatin1().constData();
        return false;
    }
    mProcFree = (PROC_FREE)mLibrary->resolve("destroy");
    if(!mProcFree)
    {
        qDebug() << QString("Can't resolve function [%1]").arg("destroy").toLatin1().constData();
        return false;
    }

    // Initialize plugin
    if(!initialize())
        return false;

    // Get match suffixes
    QString matchSuffixes = matchSuffix();
    mMatchSuffixes = matchSuffixes.split(" ");
    if(mMatchSuffixes.isEmpty())
        return false;

    // Plugin information
    pluginInfo(&mPluginInfo);

    mFileName = pluginName;

    return true;
}

bool TBackendPlugin::openTrack(TTrackInfo *track)
{
    if(mProcLoadTrack)
        return mProcLoadTrack(track);

    return false;
}

void TBackendPlugin::closeTrack()
{
    if(mProcCloseTrack)
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
    if(mProcParse)
        return mProcParse((char*)file.toLocal8Bit().constData(), musicInfo);

    return false;
}

TPluginInfo *TBackendPlugin::pluginInfo()
{
    return &mPluginInfo;
}

