#ifndef TCONTROLLER_H
#define TCONTROLLER_H

#include "playercontroller.h"
#include "playlistcontroller.h"
#include "equalizercontroller.h"

class TMainController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TMainController(QObject *parent = 0);
    ~TMainController();

    bool joint(TGuiManager *manager, TCore *core);

signals:

private slots:
    void slotQuitApp();

private:
    TMainWindow *mMainWindow;

    TPlayerController *mPlayerController;
    TPlaylistController *mPlaylistController;
    TEqualizerController *mEqualizerController;
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TCONTROLLER_H
