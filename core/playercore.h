#ifndef TPLAYERCORE_H
#define TPLAYERCORE_H

#include "pch.h"

#include "playthread.h"

#include "front/directsoundfront.h"
#include "front/sdlfront.h"

#include "pluginmanager/backendpluginmanager.h"

struct TTrackInfo
{
    QString displayName;
    QString dataFile;
    QString headerFile;
    int trackIndex;
    int start;
    int end;
    int duration;
};

typedef QList<TTrackInfo*> TTrackInfoList;

struct SpectrumInfo
{

};

typedef void (*TPlayCallback)(int played, int total, void *data);

class TPlayerCore
{
public:
    TPlayerCore();
    ~TPlayerCore();

    TMusicInfo *parse(QString fileName);

    void setTrack(TTrackInfo *track);

    void setCallback(TPlayCallback callback);

    void play();
    void stop();
    void pause();

    void enableBallence(bool enable=true);
    void enable3DEffect(bool enable=true);

    void setAmplification(int value);

    void setSpectrumAmp(int index, int value);

private:
    TPlayCallback *mCallback;
    TBackendPlugin *mBackend;
    TDirectSoundFront *mFront;

    TPlayThread *mPlayThread;
    TBackendPluginManager *mPluginManager;

    void destroyPlayThread();
};

#endif // TPLAYERCORE_H
