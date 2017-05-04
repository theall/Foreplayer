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
#include "playlistmodel.h"

#define RecordCurrentItem PlayListItem currentItem = mCore->getPlaylistItem(mCurrentIndex)
#define RestoreCurrentItem mCurrentIndex = mCore->getPlaylistItemIndex(currentItem)
#define RecordPlayingItem PlayListItem playingItem = mCore->getPlaylistItem(mPlayingIndex)
#define RestorePlayingItem \
    mPlayingIndex = mCore->getPlaylistItemIndex(playingItem);\
    mCore->setPlayingIndex(IT_PL, mPlayingIndex)

TPlaylistModel::TPlaylistModel(QObject *parent) :
    TAbstractModel(parent)
{

}

void TPlaylistModel::setCurrentIndex(int index)
{
    TAbstractModel::setCurrentIndex(index);
}

int TPlaylistModel::add(QString name)
{
    if(!mCore)
        return -1;

    int index = mCore->playlistCount();
    beginInsertRows(QModelIndex(), index, index);
    int ret = mCore->insertPlaylist(name);
    endInsertRows();

    return ret;
}

void TPlaylistModel::remove(int index)
{
    if(!mCore)
        return;

    if(index<0 || index>=mCore->playlistCount())
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    beginRemoveRows(QModelIndex(), index, index);
    mCore->removePlaylist(index);
    endRemoveRows();

    RestoreCurrentItem;
    RestorePlayingItem;
}

void TPlaylistModel::rename(int index, QString newName)
{
    if(!mCore)
        return;

    if(index<0 || index>=mCore->playlistCount())
        return;

    mCore->renamePlaylist(index, newName);
}

void TPlaylistModel::sortItems()
{
    if(!mCore)
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    mCore->sort();

    RestoreCurrentItem;
    RestorePlayingItem;

    update();
}

int TPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mCore)
        return mCore->playlistCount()+1;

    return 0;
}

int TPlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant TPlaylistModel::data(const QModelIndex &index, int role) const
{
    if(mCore)
    {
        if(role==Qt::DisplayRole || role==Qt::EditRole)
        {
            int row = index.row();
            if(row < mCore->playlistCount())
                return mCore->getPlaylistName(row);
        } else if (role==Qt::TextAlignmentRole) {
            return QVariant(Qt::AlignLeft|Qt::AlignVCenter);
        }
    }

    return TAbstractModel::data(index, role);
}

bool TPlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        mCore->setPlaylistName(index.row(), value.toString());
    }
    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TPlaylistModel::flags(const QModelIndex &index) const
{
    if(mCore && index.row()<mCore->playlistCount())
    {
        return TAbstractModel::flags(index)|Qt::ItemIsEditable;
    }
    return Qt::NoItemFlags;
}

void TPlaylistModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    if(!mCore)
        return;

    RecordCurrentItem;
    RecordPlayingItem;

    indexesMoved = mCore->movePlaylists(indexes, pos);

    RestoreCurrentItem;
    RestorePlayingItem;

    endResetModel();
}
