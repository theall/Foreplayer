#ifndef TPLAYERCONTROLLER_H
#define TPLAYERCONTROLLER_H

#include "abstractcontroller.h"

#include <QMutex>

class TPlayerController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TPlayerController(QObject *parent = 0);
    ~TPlayerController();

    void joint(TGuiManager *gui, TCore *core);

signals:
    void requestCurrentIndex(int *pi, int *mi, int *ti);
    void requestUpdateModelsPlayingIndex(int pi, int mi, int ti);
    void requestFixDuration(int microSeconds);

public slots:
    void slotRequestPlay(int pIndex, int mIndex, int tIndex);

private slots:
    void slotPlayButtonClicked();
    void slotPauseButtonClicked();
    void slotPrevButtonClicked();
    void slotNextButtonClicked();
    void slotStopButtonClicked();
    void slotVolumeValueChanged(float value);
    void slotVolumeToggled(bool toggled);

    // Internal use
    void delayStopTimer();

private:
    TPlayerCore *mPlayerCore;
    TPlaylistCore *mPlaylistCore;
    TMainWindow *mMainWindow;
    TTrackItem *mCurrentItem;

    void resetVisualWidget();
    void updateWindowTitles();

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYERCONTROLLER_H
