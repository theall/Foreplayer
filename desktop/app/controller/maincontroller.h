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
#ifndef TMAINCONTROLLER_H
#define TMAINCONTROLLER_H

#include "playercontroller.h"
#include "playlistcontroller.h"
#include "equalizercontroller.h"
#include "optionscontroller.h"

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
    TOptionsController *mOptionsController;

protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TMAINCONTROLLER_H
