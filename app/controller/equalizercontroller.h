#ifndef TEQUALIZERCONTROLLER_H
#define TEQUALIZERCONTROLLER_H

#include "abstractcontroller.h"

class TEqualizerController : public TAbstractController
{
    Q_OBJECT
public:
    explicit TEqualizerController(QObject *parent = 0);
    ~TEqualizerController();

    bool joint(TGuiManager *gui, TCore *core);

private slots:
    void slotBallanceValueChanged(float value);
    void slot3DEffectValueChanged(float value);
    void slotAmplifyValueChanged(float value);
    void slotSpectrumValueChanged(int index, float value);
    void slotEqualizerSwitchToggled(bool enabled);

private:
    TPlayerCore *mPlayerCore;
    TEqualizerWindow *mEqWindow;

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TEQUALIZERCONTROLLER_H
