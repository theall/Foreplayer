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
#include "musiclistmodel.h"

#define RecordCurrentItem MusicItem currentItem = mCore->getMusicItem(mPlaylistItem, mCurrentIndex)
#define RestoreCurrentItem mCurrentIndex = mCore->getMusicItemIndex(mPlaylistItem, currentItem)
#define RecordPlayingItem MusicItem playingItem = mCore->getMusicItem(mPlaylistItem, mPlayingIndex)
#define RestorePlayingItem mPlayingIndex = mCore->getMusicItemIndex(mPlaylistItem, playingItem);

TMusiclistModel::TMusiclistModel(QObject *parent) :
    TAbstractModel(parent)
  , mPlaylistItem(NULL)
{
}

void TMusiclistModel::setPlayListItem(PlayListItem item)
{
    if(mPlaylistItem == item)
        return;

    mCurrentIndex = -1;
    mPlayingIndex = -1;
    beginResetModel();
    mPlaylistItem = item;
    endResetModel();

    emit requestAdjustColumnWidth();
}

int TMusiclistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(mCore)
        return mCore->getMusicItemCount(mPlaylistItem)+1;

    return 0;
}

int TMusiclistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant TMusiclistModel::data(const QModelIndex &index, int role) const
{
    if(mCore)
    {
        if(role==Qt::DisplayRole || role==Qt::EditRole)
        {
            int column = index.column();
            int row = index.row();
            MusicItem data = mCore->getMusicItem(mPlaylistItem, row);
            if(!data)
                return QVariant();

            if(column==1)
            {
                return QString("%1.").arg(index.row()+1);
            } else if (column==2) {
                return mCore->getMusicItemDisplayName(data);
            } else if (column==3) {
                return Utils::microSecToTimeStr(mCore->getMusicItemDuration(data));
            }
        } else if (role==Qt::TextAlignmentRole) {
            Qt::Alignment align;
            if (index.column()==2) {
                align = Qt::AlignLeft;
            } else {
                align = Qt::AlignRight;
            }
            return QVariant(align|Qt::AlignVCenter);
        } else if (role==Qt::ToolTipRole) {
            int row = index.row();
            MusicItem data = mCore->getMusicItem(mPlaylistItem, row);
            if(!data)
                return QVariant();

            return QString(tr("Title: %1\r\n"
                              "Type: %2\r\n"
                              "Duration: %3\r\n"
                              "Artist: %4\r\n"
                              "Album: %5\r\n"
                              "Year: %6\r\n"
                              "Filename: %7\r\n"
                              "%8") \
                           .arg(mCore->getMusicItemDisplayName(data))
                           .arg(mCore->getMusicItemType(data))
                           .arg(Utils::microSecToTimeStr(mCore->getMusicItemDuration(data)))
                           .arg(mCore->getMusicItemArtist(data))
                           .arg(mCore->getMusicItemAlbum(data))
                           .arg(mCore->getMusicItemYear(data))
                           .arg(mCore->getMusicItemFileName(data))
                           .arg(mCore->getMusicItemAdditionalInfo(data)));
        }
    }

    return TAbstractModel::data(index, role);
}

bool TMusiclistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(mCore)
    {
        if(role==Qt::EditRole && index.column()==2)// The third column is title
        {
            MusicItem musicItem = mCore->getMusicItem(mPlaylistItem, index.row());
            QString newName = value.toString();
            if(!newName.isEmpty())
                mCore->setMusicItemName(musicItem, newName);
        }
    }

    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TMusiclistModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags _flags = TAbstractModel::flags(index);
    if(mCore && index.row()<mCore->getMusicItemCount(mPlaylistItem))
    {
        if(index.column()==2)
            _flags |= Qt::ItemIsEditable;
    } else {
        _flags = Qt::NoItemFlags;
    }
    return _flags;
}

void TMusiclistModel::setCurrentIndex(int index)
{
    TAbstractModel::setCurrentIndex(index);
}

void TMusiclistModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    // Record current index
    RecordCurrentItem;
    RecordPlayingItem;

    indexesMoved = mCore->moveMusicItems(mPlaylistItem, indexes, pos);

    // Restore current index
    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}

void TMusiclistModel::insertFiles(QStringList files, int pos, QList<int> &newIndexes)
{
    if(files.count()<=0)
        return;

    MusicItems musicItems;
    for(QString file : files)
        musicItems.append(mCore->parse(file));

    insertItems(pos, musicItems, newIndexes);
}

void TMusiclistModel::insertItems(int pos, MusicItems musicItems, QList<int> &newIndexes)
{
    // Record current item
    RecordCurrentItem;
    RecordPlayingItem;

    newIndexes = mCore->insertMusicItems(mPlaylistItem, pos, musicItems);
    for(int i : newIndexes)
    {
        beginInsertRows(index(i, 0), i, i);
        endInsertRows();
    }
    // Restore current index
    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}

QList<int> TMusiclistModel::insertItems(int pos, MusicItems musicItems)
{
    QList<int> newIndexes;
    insertItems(pos, musicItems, newIndexes);
    return newIndexes;
}

void TMusiclistModel::removeSelections(QList<int> indexes)
{
    if(!mPlaylistItem)
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    QList<int> ret = mCore->removeMusicItems(mPlaylistItem, indexes);
    for(int i : ret) {
        beginRemoveRows(QModelIndex(), i, i);
        endRemoveRows();
    }
    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}

QList<int> TMusiclistModel::removeRedundant()
{
    QList<int> rowsRemoved;

    if(!mPlaylistItem)
        return rowsRemoved;

    RecordCurrentItem;
    RecordPlayingItem;

    rowsRemoved = mCore->removeRedundant(mPlaylistItem);

    for(int i : rowsRemoved) {
        beginRemoveRows(QModelIndex(), i, i);
        endRemoveRows();
    }

    RestoreCurrentItem;
    RestorePlayingItem;

    update();
    return rowsRemoved;
}

QList<int> TMusiclistModel::removeErrors()
{
    QList<int> rowsRemoved;

    if(!mPlaylistItem)
        return rowsRemoved;

    RecordCurrentItem;
    RecordPlayingItem;

    rowsRemoved = mCore->removeErrors(mPlaylistItem);

    for(int i : rowsRemoved) {
        beginRemoveRows(QModelIndex(), i, i);
        endRemoveRows();
    }

    RestoreCurrentItem;
    RestorePlayingItem;
    update();

    return rowsRemoved;
}

void TMusiclistModel::removeAll()
{
    if(!mPlaylistItem)
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    beginResetModel();
    mCore->clear(mPlaylistItem);
    endResetModel();

    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}

void TMusiclistModel::update()
{
    emit layoutChanged();
}

PlayListItem TMusiclistModel::playlistItem()
{
    return mPlaylistItem;
}

void TMusiclistModel::sortItems(SortMethod sm)
{
    if(!mPlaylistItem)
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    beginResetModel();
    mCore->sort(mPlaylistItem, sm);
    endResetModel();

    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}
