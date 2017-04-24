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
#include "pluginmodel.h"

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

TPluginModel::TPluginModel(QObject *parent) :
    QAbstractTableModel(parent)
  , mCore(NULL)
{

}

TPluginModel::~TPluginModel()
{

}

void TPluginModel::setPlayerCore(TCore *core)
{
    mCore = core;
}

QVariant TPluginModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();
//        if(row>=0 && row<mExportMissions->size())
//        {
//            TExportParam *param = (TExportParam*)mExportMissions->at(row)->data();
//            switch (col) {
//            case CI_TITLE:
//                return QString::fromWCharArray(param->fileName);
//                break;
//            case CI_PROGRESS:
//                if(param->progressTotalFrames==0)
//                    return 0;
//                return (float)param->progressCurrentFrames/param->progressTotalFrames;
//                break;
//            case CI_SOURCE:
//                return QString::fromWCharArray(param->indexName);
//                break;
//            case CI_DESTINATION:
//                return QString::fromWCharArray(param->outputPath);
//                break;
//            default:
//                break;
//            }
//        }
    }
    return QVariant();
}

int TPluginModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 0;
}

int TPluginModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return CI_COUNT;
}

QVariant TPluginModel::headerData(int section, Qt::Orientation orientation, int role) const
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
