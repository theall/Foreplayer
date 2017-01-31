#include "backendplugin.h"

TBackendPlugin::TBackendPlugin() :
    mProcParse(NULL),
    mProcLoadTrack(NULL),
    mProcCloseTrack(NULL),
    mProcNextSamples(NULL),
    mProcFree(NULL),
    mLibrary(NULL)
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
    QFileInfo fi(pluginName);
    qApp->addLibraryPath(fi.absolutePath());

    mLibrary = new QLibrary(pluginName);

    PROC_INITIALIZE initialize = (PROC_INITIALIZE)mLibrary->resolve("initialize");
    if(!initialize)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("initialize").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    PROC_MATCHSUFFIXES matchSuffix = (PROC_MATCHSUFFIXES)mLibrary->resolve("matchSuffixes");
    if(!matchSuffix)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("matchSuffixes").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    PROC_PLUGININFO pluginInfo = (PROC_PLUGININFO)mLibrary->resolve("pluginInformation");
    if(!pluginInfo)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("pluginInformation").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    mProcParse = (PROC_PARSE)mLibrary->resolve("parse");
    if(!mProcParse)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("parse").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    mProcLoadTrack = (PROC_LOADTRACK)mLibrary->resolve("loadTrack");
    if(!mProcLoadTrack)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("loadTrack").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    mProcCloseTrack = (PROC_CLOSETRACK)mLibrary->resolve("closeTrack");
    if(!mProcCloseTrack)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("closeTrack").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    mProcNextSamples = (TRequestSamples)mLibrary->resolve("nextSamples");
    if(!mProcNextSamples)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("nextSamples").arg(mLibrary->errorString()).toLocal8Bit().constData();
        return false;
    }
    mProcFree = (PROC_FREE)mLibrary->resolve("destroy");
    if(!mProcFree)
    {
        qDebug() << QString("Can't resolve function [%1], %2").arg("destroy").arg(mLibrary->errorString()).toLocal8Bit().constData();
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
    {
        std::string s = file.toStdString();
        return mProcParse(s.c_str(), musicInfo);
    }
    return false;
}

TPluginInfo *TBackendPlugin::pluginInfo()
{
    return &mPluginInfo;
}

