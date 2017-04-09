#ifndef TPLAYERCORE_H
#define TPLAYERCORE_H

enum PlayerState
{
    PS_PAUSED,
    PS_STOPED,
    PS_PLAYING
};

typedef void (*TPlayCallback)(int played, int total, void *data);

class TPlayerCore
{
public:
    TPlayerCore();
    ~TPlayerCore();

    void setCallback(TPlayCallback callback);

    void stop();
    void pause();
    bool resume();
    bool isPaused();
    bool isStoped();
    bool isPlaying();

    int playedTime();

private:
    PlayerState mPlayerState;

    void init();
    void destroyPlayThread();
};

#endif // TPLAYERCORE_H
