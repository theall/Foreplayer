#include "equalizercontroller.h"

TEqualizerController::TEqualizerController(QObject *parent) :
    TAbstractController(parent)
  , mPlayerCore(NULL)
  , mEqWindow(NULL)
{

}

TEqualizerController::~TEqualizerController()
{

}

bool TEqualizerController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    mPlayerCore = core->player();
    Q_ASSERT(mPlayerCore);

    mEqWindow = gui->equalizerWindow();
    Q_ASSERT(mEqWindow);

    connect(mEqWindow, SIGNAL(eqEnableToggled(bool)), this, SLOT(slotEqualizerSwitchToggled(bool)));
    connect(mEqWindow, SIGNAL(eqBalanceChanged(float)), this, SLOT(slotBallanceValueChanged(float)));
    connect(mEqWindow, SIGNAL(eqSurroundChanged(float)), this, SLOT(slot3DEffectValueChanged(float)));
    connect(mEqWindow, SIGNAL(eqPrempChanged(float)), this, SLOT(slotAmplifyValueChanged(float)));
    connect(mEqWindow, SIGNAL(eqFactorChanged(int,float)), this, SLOT(slotSpectrumValueChanged(int,float)));
    return TAbstractController::joint(gui, core);;
}

void TEqualizerController::slotBallanceValueChanged(float value)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_BALLANCE, value);
}

void TEqualizerController::slot3DEffectValueChanged(float value)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_EFFECT, value);
}

void TEqualizerController::slotAmplifyValueChanged(float value)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_AMPLIFICATION, value);
}

void TEqualizerController::slotSpectrumValueChanged(int index, float value)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_EQUALIZER_FACTOR, value, index);
}

void TEqualizerController::slotEqualizerSwitchToggled(bool enabled)
{
    if(mPlayerCore)
        mPlayerCore->setAudioParameter(AP_EQUALIZER_ENABLE, (float)enabled);
}

void TEqualizerController::slotTimerEvent()
{

}
