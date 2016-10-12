#ifndef TPLAYLISTCONTROLLER_H
#define TPLAYLISTCONTROLLER_H

#include "pch.h"
#include "../core/playlistcore.h"
#include "../windows/tplaylistwindow.h"

class TPlaylistController : public QObject
{
    Q_OBJECT
public:
    explicit TPlaylistController(QObject *parent = 0);

    void joint(TPlaylistWindow *window, TPlaylistCore *core);

signals:

public slots:

private:
    TPlaylistWindow *mWindow;
    TPlaylistCore *mCore;
};

#endif // TPLAYLISTCONTROLLER_H
