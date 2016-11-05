#ifndef TCONTROLLER_H
#define TCONTROLLER_H

#include "playlistcontroller.h"

class TThread : public QThread
{
    Q_OBJECT

public:
    explicit TThread(TGuiManager* window=0);
    ~TThread(){ }

    // QThread interface
protected:
    void run() Q_DECL_OVERRIDE;

private slots:
    void playClicked()
    {
        mPlaying = true;
    }
    void pauseClicked()
    {
        mPlaying = false;
    }
private:
    TGuiManager* mManager;
    bool mPlaying;
};

class TMainController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TMainController(QObject *parent = 0);
    ~TMainController();

    void joint(TGuiManager *manager, TCore *core);

signals:

private slots:

private:
    TThread *mThread;
    TGuiManager *mManager;
    TMainWindow *mMainWindow;
    TPlayerCore *mPlayerCore;
    TCore *mCore;
    TPlaylistController *mPlaylistController;

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TCONTROLLER_H
