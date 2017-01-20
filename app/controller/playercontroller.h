#ifndef TPLAYERCONTROLLER_H
#define TPLAYERCONTROLLER_H

#include "abstractcontroller.h"

class TPlayerController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TPlayerController(QObject *parent = 0);

    void joint(TGuiManager *gui, TCore *core);

signals:

public slots:
    void slotRequestPlay(int pIndex, int mIndex, int tIndex);

private:
    TPlayerCore *mPlayerCore;
    TPlaylistCore *mPlaylistCore;
    TMainWindow *mMainWindow;

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYERCONTROLLER_H
