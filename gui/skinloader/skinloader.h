#ifndef SKINLOADER_H
#define SKINLOADER_H

#include "skinutils.h"

class SkinLoader : public QObject
{
    Q_OBJECT
public:
    explicit SkinLoader(QObject *parent = 0);

    void load(QString fileName);

    PlayerWindowParam *mainWindow();
    PlayerWindowParam *miniWindow();
    LyricWindowParam *lyricWindow();
    EqualizerWindowParam *equalizerWindow();
    PlaylistWindowParam *playlistWindow();
    BrowserWindowParam *browserWindow();
    DesklrcBarParam *desklrcBar();

signals:

public slots:

private:
    QString mSkinPath;
    SkinDesc m_skinAttr;
    PlayerWindowParam mPlayerWindow;
    LyricWindowParam mLyricWIndow;
    EqualizerWindowParam mEqualizerWindow;
    PlaylistWindowParam mPlaylistWindow;
    PlayerWindowParam mMiniWindow;
    BrowserWindowParam mBrowserWindow;
    DesklrcBarParam mDesklrcBar;

    void parseSkinNode(QXmlStreamReader &xml);
    void parsePlayerWindow(QXmlStreamReader &xml);
    QPixmap loadPixmap(QString imageName);
    QIcon loadIcon(QString iconFile);
    void parseElement(QXmlStreamReader &xml, Element &element);
    void parseWindowParam(QXmlStreamReader &xml, WindowParam &window);
    void parseLyricWindow(QXmlStreamReader &xml);
    void parseEqualizerWindow(QXmlStreamReader &xml);
    void parsePlaylistWindow(QXmlStreamReader &xml);
    void parseMiniWindow(QXmlStreamReader &xml);
    void parseBrowserWindow(QXmlStreamReader &xml);
    void parseDesklrcBar(QXmlStreamReader &xml);
};

#endif // SKINLOADER_H
