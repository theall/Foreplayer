#ifndef SKINUTILS_H
#define SKINUTILS_H

#include "pch.h"

class TPosition
{
public:
    TPosition();
    TPosition(QString s);

    void setString(QString s);
    QRect rect();
    QRect geometry();

private:
    QRect mRect;
    QRect mGeometry;
};

struct SkinDesc
{
    QString version;
    QString name;
    QString author;
    QString url;
    QString email;
    QColor transparent;
};

struct WindowParam
{
    QPixmap background;
    TPosition position;
    TPosition resize;
    int interval;
    QColor transparent;

    WindowParam operator = (WindowParam param)
    {
        background = param.background;
        position = param.position;
        resize = param.resize;
        interval = param.interval;
        transparent = param.transparent;

        return *this;
    }
};

struct Element
{
    TPosition position;
    QPixmap image;
    QPixmap image1;
    QPixmap image2;
    QIcon icon;
    bool vertical;
    QColor color;
    QFont font;
    Qt::Alignment alignment;
    int interval;
};

struct PlayerWindowParam
{
    WindowParam window;
    Element play;
    Element pause;
    Element prev;
    Element next;
    Element stop;
    Element open;
    Element mute;
    Element lyric;
    Element equalizer;
    Element playlist;
    Element browser;
    Element minimize;
    Element minimode;
    Element exit;
    Element progress;
    Element volume;
    Element icon;
    Element info;
    Element led;
    Element stereo;
    Element status;
    Element visual;

    PlayerWindowParam operator = (PlayerWindowParam param)
    {
        window = param.window;
        play = param.play;
        pause = param.pause;
        prev = param.prev;
        next = param.next;
        stop = param.stop;
        open = param.open;
        mute = param.mute;
        lyric = param.lyric;
        equalizer = param.equalizer;
        playlist = param.playlist;
        browser = param.browser;
        minimize = param.minimize;
        minimode = param.minimode;
        exit = param.exit;
        progress = param.progress;
        volume = param.volume;
        icon = param.icon;
        info = param.info;
        led = param.led;
        stereo = param.stereo;
        status = param.status;
        visual = param.visual;

        return *this;
    }
};

struct VisualEffectParam
{
    QColor specTop;
    QColor specMiddle;
    QColor specBottom;
    QColor specPick;
    int specWide;
};

struct LyricWindowParam
{
    WindowParam window;
    Element minimize;
    Element close;
    Element ontop;
    Element desklyc;
};

struct EqualizerWindowParam
{
    WindowParam window;
    Element close;
    Element enabled;
    Element profile;
    Element reset;
    Element balance;
    Element surround;
    Element preamp;
    Element eqfactor;
};

struct PlaylistWindowParam
{
    WindowParam window;
    Element close;
    Element toolbar;
    Element scrollbar;
    Element playlist;
};

struct BrowserWindowParam
{
    WindowParam window;
    Element close;
    Element backward;
    Element forward;
    Element refresh;
    Element startup;
    Element linktxt;
    Element browser;
    Element minibtn;
    Element downloadbtn;
};

struct DesklrcBarParam
{
    WindowParam window;
    Element desklrc_bar;
    Element icon;
    Element play;
    Element pause;
    Element prev;
    Element next;
    Element list;
    Element settings;
    Element kalaok;
    Element lines;
    Element lock;
    Element ontop;
    Element back;
    Element close;
};

bool str2bool(QString s);

Qt::Alignment str2Align(QString s);

#endif // SKINUTILS_H
