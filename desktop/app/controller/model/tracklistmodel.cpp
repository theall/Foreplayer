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
#include "tracklistmodel.h"

#include "utils.h"

enum ColumnIndex
{
    CI_STUB = 0,
    CI_NUMBER,
    CI_TITLE,
    CI_DURATION,
    CI_COUNT
};

int strToMSecs(QString s)
{
    QStringList sl = s.split(":");
    int length = sl.size();
    int hour = 0;
    int minute = 0;
    int sec = 0;
    if(length > 0)
    {
        sec = sl[length-1].toInt();
        if(length > 1)
        {
            minute = sl[length-2].toInt();
            if(length > 2)
                hour = sl[length-3].toInt();
        }
    }
    return (hour*3600+minute*60+sec)*1000;
}

TTrackListModel::TTrackListModel(QObject *parent) :
    TAbstractModel(parent)
  , mMusicItem(NULL)
{

}

void TTrackListModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    Q_UNUSED(indexes);
    Q_UNUSED(pos);
    Q_UNUSED(indexesMoved);
}

void TTrackListModel::setMusicItem(MusicItem item)
{
    if(mMusicItem==item)
        return;

    mCurrentIndex = -1;
    mPlayingIndex = -1;
    beginResetModel();
    mMusicItem = item;
    endResetModel();

    emit requestAdjustColumnWidth();
}

MusicItem TTrackListModel::musicItem()
{
    return mMusicItem;
}

int TTrackListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mCore)
        return mCore->getTrackItemCount(mMusicItem)+1;

    return 0;
}

int TTrackListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant TTrackListModel::data(const QModelIndex &index, int role) const
{
    if(mCore)
    {
        int column = index.column();
        int row = index.row();
        TrackItem trackItem = mCore->getTrackItem(mMusicItem, row);
        if(!trackItem)
            return QVariant();

        if(role==Qt::DisplayRole || role==Qt::EditRole)
        {
            if(column==CI_NUMBER)
            {
                return QString::number(row+1) + ".";
            } else if (column==CI_TITLE) {
                return mCore->getTrackItemName(trackItem);
            } else if (column==CI_DURATION) {
                return Utils::microSecToTimeStr(mCore->getTrackItemDuration(trackItem));
            }
        } else if (role==Qt::TextAlignmentRole) {
            Qt::Alignment align;
            if (column==CI_TITLE) {
                align = Qt::AlignLeft;
            } else {
                align = Qt::AlignRight;
            }
            return QVariant(align|Qt::AlignVCenter);
        } else if (role==Qt::TextColorRole) {
            if(!mCore->isTrackItemEnabled(trackItem))
            {
                return QColor("#3c3c3c");
            }
        } else if (role==Qt::ToolTipRole) {
            return QString(tr("Index: %1\r\nName: %2\r\nDuration: %3\r\n%4") \
                           .arg(mCore->getTrackItemIndexName(trackItem)) \
                           .arg(mCore->getTrackItemName(trackItem)) \
                           .arg(Utils::microSecToTimeStr(mCore->getTrackItemDuration(trackItem))) \
                           .arg(mCore->getTrackItemAdditionalInfo(trackItem)));
        }
    }

    return TAbstractModel::data(index, role);
}

bool TTrackListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int col = index.column();
        int row = index.row();
        TrackItem trackItem = mCore->getTrackItem(mMusicItem, row);
        if(trackItem)
        {
            if(col==CI_TITLE)
            {
                // Title
                QString newName = value.toString();
                if(!newName.isEmpty())
                    mCore->setTrackItemName(mMusicItem, trackItem, newName);
            } else if (col==CI_DURATION) {
                // Duration
                int newDuration = strToMSecs(value.toString());
                if(mCore->setTrackItemDuration(mMusicItem, trackItem, newDuration))
                    emit requestAdjustColumnWidth();
            }
        }
    }
    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TTrackListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags _flags = TAbstractModel::flags(index);
    if(mCore && index.row()<mCore->getTrackItemCount(mMusicItem))
    {
        // Title and length columns are editable
        if(index.column()>1)
            _flags |= Qt::ItemIsEditable;
    } else {
        _flags = Qt::NoItemFlags;
    }
    return _flags;
}
