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
