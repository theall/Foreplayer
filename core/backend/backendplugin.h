#ifndef BACKENDPLUGIN_H
#define BACKENDPLUGIN_H

#include "pch.h"
#include "../../plugins/backend/backendinterface.h"

// Initialize plugin
bool (*PROC_INITIALIZE)();

// Verify this plugin can parse specify suffix of file
char *(*PROC_MATCHSUFFIXES)();
bool (*PROC_PARSE)(char* file, TMusicInfo* musicInfo);

bool (*PROC_LOADTRACK)(TTrackInfo* track);
void (*PROC_CLOSETRACK)();

void (*PROC_PLUGININFO)(TPluginInfo *pluginInfo);

bool (*PROC_FREE)();

class TBackendPlugin
{
public:
    explicit TBackendPlugin();
    ~TBackendPlugin();

    bool load(QString pluginName);
    bool openTrack(TTrackInfo* track);
    bool closeTrack();

    TRequestSamples getCallback();

    bool matchSuffix(QString suffix);
    bool parse(QString file, TMusicInfo* musicInfo);

    TPluginInfo *pluginInfo();

private:
    PROC_PARSE mProcParse;
    PROC_LOADTRACK mProcLoadTrack;
    PROC_CLOSETRACK mProcCloseTrack;
    TRequestSamples mProcNextSamples;
    PROC_FREE mProcFree;

    TPluginInfo mPluginInfo;
    QStringList mMatchSuffixes;

    QLibrary *mLibrary;
    QString mFileName;
};
#endif // BACKENDPLUGIN_H
