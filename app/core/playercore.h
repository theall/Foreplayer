#ifndef TPLAYERCORE_H
#define TPLAYERCORE_H

#include "playthread.h"

#include "front/sdlfront.h"

#include "pluginmanager/backendpluginmanager.h"
#include "playlist/playlistitem.h"

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

    bool playTrack(TTrackItem *track);

    void setCallback(TPlayCallback callback);

    void stop();
    void pause();
    bool resume();

    int playedTime();

    void setAudioParameter(TAudioParameter type, float value, int param=0);
    void getAudioData(TAudioDataType dataType, void *param1, void* param2);

private:
    TPlayThread *mPlayThread;
    TBackendPluginManager *mPluginManager;

    void init();
    void destroyPlayThread();
};

#endif // TPLAYERCORE_H
