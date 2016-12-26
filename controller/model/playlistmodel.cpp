#include "playlistmodel.h"

TPlaylistModel::TPlaylistModel(QObject *parent, TPlaylistItems *playlist) :
    TAbstractModel(parent),
    mPlaylist(playlist)
{

}

void TPlaylistModel::setPlaylistCore(TPlaylistCore *core)
{
    if(core)
        mPlaylist = core->playlists();

    TAbstractModel::setPlaylistCore(core);
}

void TPlaylistModel::setCurrentIndex(int index)
{
    mPlaylistCore->setCurrentIndex(PlayList, index);

    TAbstractModel::setCurrentIndex(index);
}

void TPlaylistModel::add(QString name)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    int index = mPlaylist->size();
    beginInsertRows(QModelIndex(), index, index);
    mPlaylistCore->insertPlaylist(name);
    endInsertRows();
}

void TPlaylistModel::move(int from, int to)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    int index = mPlaylist->size();
    if(from<0 || from>=index)
        return;

    if(to < 0)
        to = 0;
    if(to >= index)
        to = index;

    //beginMoveRows(QModelIndex(), from, to);
    //endMoveRows();
}

void TPlaylistModel::remove(int index)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    if(index<0 || index>=mPlaylist->size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    mPlaylistCore->removePlaylist(index);
    endRemoveRows();
}

int TPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mPlaylist)
        return mPlaylist->size();

    return 0;
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
        if(mPlaylist && row>-1 && row<mPlaylist->size())
            return mPlaylist->at(row)->displayName;
    } else if (role==Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter);
    }
    return TAbstractModel::data(index, role);
}

bool TPlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int row = index.row();
        if(mPlaylist && row>-1 && row<mPlaylist->size())
            mPlaylist->at(row)->displayName = value.toString();
    }
    return TAbstractModel::setData(index, value, role);
}
