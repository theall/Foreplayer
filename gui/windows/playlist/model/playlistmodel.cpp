#include "playlistmodel.h"

TPlaylistModel::TPlaylistModel(QObject *parent) :
    TAbstractModel(parent)
{
    for(int i=0;i<20;i++)
    {
        addPlaylist("list"+QString::number(i));
    }
}

void TPlaylistModel::addPlaylist(QString name)
{
    int index = mPlaylist.size();
    beginInsertRows(QModelIndex(), index, index);
    mPlaylist.append(name);
    endInsertRows();
}

void TPlaylistModel::removePlaylist(int index)
{
    if(index>-1 && index<mPlaylist.size())
    {
        beginRemoveRows(QModelIndex(), index, index);
        mPlaylist.removeAt(index);
        endRemoveRows();
    }
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

    return TAbstractModel::data(index, role);
}

bool TPlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int row = index.row();
        if(row>-1 && row<mPlaylist.size())
            mPlaylist[row] = value.toString();
    }
    return TAbstractModel::setData(index, value, role);
}
