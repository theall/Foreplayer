#ifndef TPLAYERCORE_H
#define TPLAYERCORE_H

#include "playthread.h"

#include "front/directsoundfront.h"
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

    void enableBallence(bool enable=true);
    void enable3DEffect(bool enable=true);

    void setAmplification(int value);

    void setSpectrumAmp(int index, int value);

private:
    TPlayThread *mPlayThread;
    TBackendPluginManager *mPluginManager;

    void init();
    void destroyPlayThread();
};

#endif // TPLAYERCORE_H
