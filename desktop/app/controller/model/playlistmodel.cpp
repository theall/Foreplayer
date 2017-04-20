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

    beginRemoveRows(QModelIndex(), index, index);
    mCore->removePlaylist(index);
    endRemoveRows();

    if(index < mCurrentIndex)
    {
        mCurrentIndex--;

        if(mCurrentIndex < 0)
            mCurrentIndex = 0;
    }

    int newSize = mCore->playlistCount();
    if(mCurrentIndex >= newSize)
        mCurrentIndex = newSize - 1;
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

    PlayListItem currentItem = mCore->getPlaylistItem(mCurrentIndex);
    mCore->sort();
    mCurrentIndex = mCore->getPlaylistItemIndex(currentItem);

    update();
}

int TPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mCore)
        return mCore->playlistCount();

    return 0;
}

int TPlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant TPlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole || role==Qt::EditRole)
    {
        return mCore->getPlaylistName(index.row());
    } else if (role==Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter);
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
    return TAbstractModel::flags(index)|Qt::ItemIsEditable;
}

void TPlaylistModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    if(!mCore)
        return;

    // Find and record current index
    PlayListItem currentItem = mCore->getPlaylistItem(mCurrentIndex);

    indexesMoved = mCore->movePlaylists(indexes, pos);

    // Restore current index
    if(currentItem)
    {
        setCurrentIndex(mCore->getPlaylistItemIndex(currentItem));
    }

    endResetModel();
}
