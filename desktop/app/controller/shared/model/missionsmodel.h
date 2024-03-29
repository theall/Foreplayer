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
#ifndef TMISSIONSMODEL_H
#define TMISSIONSMODEL_H

#include <QMutex>
#include <QSharedMemory>
#include <QAbstractTableModel>

typedef QList<QSharedMemory*> TExportMissions;

class TMissionsModel : public QAbstractTableModel
{
public:
    TMissionsModel(QObject *parent = 0, TExportMissions *exportMissions = 0, QMutex *mutex = 0);
    ~TMissionsModel();

    void update();
    void addMissions(TExportMissions missions);

    QList<int> removeMissions(QList<int> indexes);
    QList<int> startMissions(QList<int> indexes);
    QList<int> pauseMissions(QList<int> indexes);
    QList<int> restartMissions(QList<int> indexes);
    QList<int> clearCompletedMissions();

private:
    TExportMissions *mExportMissions;
    QMutex *mExportMissionsLock;

    QList<int> changeMissionsState(QList<int> indexes, int checkState, int newState);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
};

#endif // TMISSIONSMODEL_H
