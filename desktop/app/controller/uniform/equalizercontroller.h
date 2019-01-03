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
    TEqualizerWindow *mEqWindow;

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TEQUALIZERCONTROLLER_H
