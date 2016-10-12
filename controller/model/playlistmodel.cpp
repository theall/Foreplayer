#include "playlistmodel.h"

TPlaylistModel::TPlaylistModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int TPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mPlaylist.size();
}

int TPlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant TPlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int row = index.row();
        if(row>-1 && row<mPlaylist.size())
            return mPlaylist[row];
    }

    return QAbstractListModel::data(index, role);
}

bool TPlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int row = index.row();
        if(row>-1 && row<mPlaylist.size())
            mPlaylist[row] = value.toString();
    }
    return QAbstractListModel::setData(index, value, role);
}
