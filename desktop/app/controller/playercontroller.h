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

    bool joint(TGuiManager *gui, TCore *core);

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
    void slotPrevMusicTriggered();
    void slotNextMusicTriggered();

    // Internal use
    void delayStopTimer();

private:
    TMainWindow *mMainWindow;
    TrackItem mCurrentItem;

    void resetVisualWidget();
    void updateWindowTitles();
    void decidePlayNext();
    void getNextPlayindex(int *pIndex, int *mIndex, int *tIndex);

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TPLAYERCONTROLLER_H
