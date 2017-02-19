#ifndef BACKENDPLUGIN_H
#define BACKENDPLUGIN_H

#include "pch.h"
#include "../../../plugins/backend/backendinterface.h"

// Initialize plugin
typedef bool (*PROC_INITIALIZE)();

// Verify this plugin can parse specify suffix of file
typedef char *(*PROC_MATCHSUFFIXES)();
typedef bool (*PROC_PARSE)(const wchar_t *file, TMusicInfo* musicInfo);

typedef bool (*PROC_LOADTRACK)(TTrackInfo* track);
typedef void (*PROC_CLOSETRACK)();

typedef void (*PROC_PLUGININFO)(TPluginInfo *pluginInfo);

typedef bool (*PROC_FREE)();
typedef int (*PROC_SAMPLESIZE)(int sampleRate, int fps);

class TBackendPlugin
{
public:
    explicit TBackendPlugin();
    ~TBackendPlugin();

    bool load(QString pluginName);
    bool openTrack(TTrackInfo* track);
    void closeTrack();

    TRequestSamples getCallback();

    bool matchSuffix(QString suffix);
    bool parse(QString file, TMusicInfo* musicInfo);

    int getSampleSize(int sampleRate, int fps);

    TPluginInfo *pluginInfo();

private:
    PROC_PARSE mProcParse;
    PROC_LOADTRACK mProcLoadTrack;
    PROC_CLOSETRACK mProcCloseTrack;
    TRequestSamples mProcNextSamples;
    PROC_SAMPLESIZE mProcSampleSize;
    PROC_FREE mProcFree;

    TPluginInfo mPluginInfo;
    QStringList mMatchSuffixes;

    QLibrary *mLibrary;
    QString mFileName;
};
#endif // BACKENDPLUGIN_H
