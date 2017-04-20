/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "equalizercontroller.h"

TEqualizerController::TEqualizerController(QObject *parent) :
    TAbstractController(parent)
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
    if(mCore)
        mCore->setAudioParameter(AP_BALLANCE, value);
}

void TEqualizerController::slot3DEffectValueChanged(float value)
{
    if(mCore)
        mCore->setAudioParameter(AP_EFFECT, value);
}

void TEqualizerController::slotAmplifyValueChanged(float value)
{
    if(mCore)
        mCore->setAudioParameter(AP_AMPLIFICATION, value);
}

void TEqualizerController::slotSpectrumValueChanged(int index, float value)
{
    if(mCore)
        mCore->setAudioParameter(AP_EQUALIZER_FACTOR, value, index);
}

void TEqualizerController::slotEqualizerSwitchToggled(bool enabled)
{
    if(mCore)
        mCore->setAudioParameter(AP_EQUALIZER_ENABLE, (float)enabled);
}

void TEqualizerController::slotTimerEvent()
{

}
