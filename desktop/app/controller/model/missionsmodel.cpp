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

#include <QMutexLocker>

enum ColumnIndex
{
    CI_TITLE=0,
    CI_INDEX,
    CI_PROGRESS,
    CI_SOURCE,
    CI_DESTINATION,
    CI_STATUS,
    CI_COUNT
};

#define LOCK(x) QMutexLocker locker(x);\
    Q_UNUSED(locker)

TMissionsModel::TMissionsModel(QObject *parent, TExportMissions *exportMissions, QMutex *mutex) :
    QAbstractTableModel(parent)
  , mExportMissions(exportMissions)
  , mExportMissionsLock(mutex)
{

}

TMissionsModel::~TMissionsModel()
{

}

void TMissionsModel::update()
{
    emit layoutChanged();
}

void TMissionsModel::addMissions(TExportMissions missions)
{
    if(!mExportMissions)
        return;

    mExportMissionsLock->lock();
    *mExportMissions += missions;
    mExportMissionsLock->unlock();

    update();
}

QList<int> TMissionsModel::removeMissions(QList<int> indexes)
{
    QList<int> ret;

    if(!mExportMissions || indexes.size() < 1)
        return ret;

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b){
        return a > b;
    });

    ret = changeMissionsState(indexes, ES_ALL, ES_COMPLETE);

    mExportMissionsLock->lock();
    for(int i : ret)
    {
        delete mExportMissions->at(i);
        mExportMissions->removeAt(i);
    }
    mExportMissionsLock->unlock();
    update();
    return ret;
}

QList<int> TMissionsModel::startMissions(QList<int> indexes)
{
    QList<int> ret = changeMissionsState(indexes, ES_PAUSED, ES_RUNNING);
    update();

    return ret;
}

QList<int> TMissionsModel::pauseMissions(QList<int> indexes)
{
    QList<int> ret = changeMissionsState(indexes, ES_RUNNING, ES_PAUSED);
    update();

    return ret;
}

QList<int> TMissionsModel::clearCompletedMissions()
{
    QList<int> ret;
    if(!mExportMissions)
        return ret;

    mExportMissionsLock->lock();
    int i = 0;
    for(QSharedMemory *mission : *mExportMissions)
    {
        TExportParam *exportParam = (TExportParam*)mission->data();
        if(exportParam && exportParam->state == ES_COMPLETE)
            ret.append(i);
        i++;
    }
    for(int index : ret)
    {
        delete mExportMissions->at(index);
        mExportMissions->removeAt(index);
    }
    mExportMissionsLock->unlock();

    return ret;
}

QList<int> TMissionsModel::changeMissionsState(QList<int> indexes, ExportState checkState, ExportState newState)
{
    QList<int> ret;

    int size = mExportMissions->size();
    for(int i : indexes)
    {
        if(i<0 || i>=size)
            continue;

        TExportParam *exportParam = (TExportParam*)mExportMissions->at(i)->data();
        if(!exportParam)
            continue;

        if(checkState & exportParam->state)
            exportParam->state = newState;

        ret.append(i);
    }

    return ret;
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
                return QString::fromWCharArray(param->title);
                break;
            case CI_INDEX:
                return QString::fromWCharArray(param->indexName);
                break;
            case CI_PROGRESS:
                if(param->progressTotalFrames==0)
                    return 0;
                return (float)param->progressCurrentFrames/param->progressTotalFrames;
                break;
            case CI_SOURCE:
                return QString::fromWCharArray(param->fileName);
                break;
            case CI_DESTINATION:
                return QString::fromWCharArray(param->outputPath);
                break;
            case CI_STATUS:
                switch (param->state) {
                case ES_NULL:
                    return tr("Initialized");
                        break;
                case ES_READY:
                    return tr("Wait process start");
                    break;
                case ES_STARTING:
                    return tr("Process starting");
                    break;
                case ES_RUNNING:
                    return tr("Running");
                    break;
                case ES_ERROR:
                    return QString::fromWCharArray(param->errorString);
                    break;
                case ES_PAUSED:
                    return tr("Paused");
                    break;
                case ES_COMPLETE:
                    return tr("Completed");
                    break;
                default:
                    break;
                }
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
        case CI_TITLE:
            return tr("Title");
            break;
        case CI_INDEX:
            return tr("Index");
            break;
        case CI_PROGRESS:
            return tr("Progress");
            break;
        case CI_SOURCE:
            return tr("Source");
            break;
        case CI_DESTINATION:
            return tr("Destination");
            break;
        case CI_STATUS:
            return tr("Status");
            break;
        default:
            break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
