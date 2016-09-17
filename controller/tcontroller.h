#ifndef TCONTROLLER_H
#define TCONTROLLER_H

#include "tguimanager.h"
#include "tplayercore.h"

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

class TController : public QObject
{
    Q_OBJECT
public:
    explicit TController(QObject *parent = 0);
    ~TController();

    void joint(TGuiManager *manager, TPlayerCore *core);

signals:

private slots:

private:
    TThread *mThread;
    TGuiManager *mManager;
    TMainWindow *mMainWindow;
    TPlayerCore *mCore;
};

#endif // TCONTROLLER_H
