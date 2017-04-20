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
#include "missionsmodel.h"

#define LABEL_TITLE         "Title"
#define LABEL_PROGRESS      "Progress"
#define LABEL_SOURCE        "Source"
#define LABEL_DESTINATION   "Destination"

enum ColumnIndex
{
    CI_TITLE=0,
    CI_PROGRESS,
    CI_SOURCE,
    CI_DESTINATION,
    CI_COUNT
};

TMissionsModel::TMissionsModel(QObject *parent, TExportMissions *exportMissions) :
    QAbstractTableModel(parent)
  , mExportMissions(exportMissions)
{

}

TMissionsModel::~TMissionsModel()
{

}

void TMissionsModel::update()
{
    emit layoutChanged();
}

void TMissionsModel::addMission(QSharedMemory *mission)
{
    if(!mExportMissions)
        return;

    int index = mExportMissions->size();
    beginInsertRows(QModelIndex(), index, index);
    mExportMissions->prepend(mission);
    endInsertRows();
}

void TMissionsModel::addMissions(TExportMissions missions)
{
    if(!mExportMissions)
        return;

    for(QSharedMemory *mission : missions) {
        addMission(mission);
    }
}

QVariant TMissionsModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole && mExportMissions)
    {
        int row = index.row();
        int col = index.column();
        if(row>=0 && row<mExportMissions->size())
        {
            TExportParam *param = (TExportParam*)mExportMissions->at(row)->data();
            switch (col) {
            case CI_TITLE:
                return QString::fromWCharArray(param->fileName);
                break;
            case CI_PROGRESS:
                if(param->progressTotalFrames==0)
                    return 0;
                return (float)param->progressCurrentFrames/param->progressTotalFrames;
                break;
            case CI_SOURCE:
                return QString::fromWCharArray(param->indexName);
                break;
            case CI_DESTINATION:
                return QString::fromWCharArray(param->outputPath);
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}

bool TMissionsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;
}

int TMissionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if(mExportMissions)
        return mExportMissions->size();
    return 0;
}

int TMissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return CI_COUNT;
}

QVariant TMissionsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if(role==Qt::DisplayRole)
    {
        switch (section) {
        case 0:
            return LABEL_TITLE;
            break;
        case 1:
            return LABEL_PROGRESS;
            break;
        case 2:
            return LABEL_SOURCE;
            break;
        case 3:
            return LABEL_DESTINATION;
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
