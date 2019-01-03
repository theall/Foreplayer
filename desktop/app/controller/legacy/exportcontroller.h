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
#ifndef TEXPORTCONTROLLER_H
#define TEXPORTCONTROLLER_H

#include "abstractcontroller.h"

#include "../shared/model/missionsmodel.h"

#include <QSharedMemory>

class TExportController : public TAbstractController
{
    Q_OBJECT

public:
    explicit TExportController(QObject *parent = 0);
    ~TExportController();

    bool joint(TGuiManager *gui, TCore *core);
    bool hasExportingMissions();

public slots:
    void slotRequestAddExportMissions(void *missions, int size);

private slots:
    void slotRequestCancelMissions(QList<int> rows);
    void slotRequestStartMissions(QList<int> rows);
    void slotRequestPauseMissions(QList<int> rows);
    void slotRequestExploreFiles(QList<int> rows);
    void slotRequestRestartMissions(QList<int> rows);
    void slotQueryMissionsStatus(QList<int> rows, QList<int> &state);
    void slotRequestCloseWindow();

private:
    TExportMissions mExportMissions;
    QMutex mExportMissionsLock;
    TMissionsModel *mMissionsModel;
    TExportMissionsDialog *mExportMissionDialog;

    // TAbstractController interface
protected slots:
    void slotTimerEvent() Q_DECL_OVERRIDE;
};

#endif // TEXPORTCONTROLLER_H
