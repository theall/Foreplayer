#ifndef TPLAYERCONTROLLER_H
#define TPLAYERCONTROLLER_H

#include "abstractcontroller.h"
#include "spectrum/spectrumanalyser.h"

class TPlayerController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TPlayerController(QObject *parent = 0);
    ~TPlayerController();

    void joint(TGuiManager *gui, TCore *core);

signals:
    void requestUpdateModelsPlayingIndex(int pi, int mi, int ti);

public slots:
    void slotRequestPlay(int pIndex, int mIndex, int tIndex);

private slots:
    void slotPlayButtonClicked();
    void slotPauseButtonClicked();
    void slotPrevButtonClicked();
    void slotNextButtonClicked();
    void slotStopButtonClicked();
    void slotSpectrumChanged(const TFrequencySpectrum &spectrum);

private:
    TPlayerCore *mPlayerCore;
    TPlaylistCore *mPlaylistCore;
    TMainWindow *mMainWindow;

    TTrackItem *mCurrentItem;

    TSpectrumAnalyser *mSpectrum;

    void updateWindowTitles();

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYERCONTROLLER_H
