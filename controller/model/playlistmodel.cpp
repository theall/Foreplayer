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
    if(index < 0)
        return;

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

void TPlaylistModel::remove(int index)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    if(index<0 || index>=mPlaylist->size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    mPlaylistCore->removePlaylist(index);
    endRemoveRows();

    if(index < mCurrentIndex)
    {
        mCurrentIndex--;

        if(mCurrentIndex < 0)
            mCurrentIndex = 0;
    }

    int newSize = mPlaylist->size();
    if(mCurrentIndex >= newSize)
        mCurrentIndex = newSize - 1;

    if(newSize <= 0)
        mCurrentIndex = -1;
}

void TPlaylistModel::rename(int index, QString newName)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    if(index<0 || index>=mPlaylist->size())
        return;

    TPlaylistItem *item = mPlaylist->at(index);
    if(item)
        item->setDisplayName(newName);
}

void TPlaylistModel::sort()
{
    mSortState = !mSortState;
    if(mSortState)
        std::sort(mPlaylist->begin(), mPlaylist->end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return a->name().toLower() > b->name().toLower();
        });
    else
        std::sort(mPlaylist->begin(), mPlaylist->end(), [=](TPlaylistItem *a, TPlaylistItem *b){
            return a->name().toLower() < b->name().toLower();
        });
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
    if(role==Qt::DisplayRole || role==Qt::EditRole)
    {
        int row = index.row();
        if(mPlaylist && row>-1 && row<mPlaylist->size())
            return mPlaylist->at(row)->name();
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
            mPlaylist->at(row)->setDisplayName(value.toString());
    }
    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TPlaylistModel::flags(const QModelIndex &index) const
{
    return TAbstractModel::flags(index)|Qt::ItemIsEditable;
}

void TPlaylistModel::moveItems(QList<int> indexes, int insertPos, QList<int> &indexesMoved)
{
    if(!mPlaylistCore || !mPlaylist)
        return;

    int listSize = mPlaylist->size();
    if(indexes.count()<=0 || listSize<=0)
        return;

    if(insertPos < 0)
        insertPos = 0;
    if(insertPos >= listSize)
        insertPos = listSize;

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b) {
        return a > b;
    });
    TPlaylistItems items;

    // Find and record current index
    int currentPlaylistIndex = mPlaylistCore->currentIndex(PlayList);
    if(currentPlaylistIndex < 0)
        currentPlaylistIndex = 0;

    if(currentPlaylistIndex >= mPlaylist->size())
        currentPlaylistIndex = mPlaylist->size() - 1;

    TPlaylistItem *currentItem = mPlaylist->at(currentPlaylistIndex);

    beginResetModel();
    foreach (int i, indexes) {
        if(i<0 || i>=listSize)
            continue;
        beginRemoveRows(index(i, 0), i, i);
        items.append(mPlaylist->takeAt(i));
        endRemoveRows();
        if(i < insertPos)
            insertPos--;
    }
    int i = 0;
    foreach (TPlaylistItem *item, items) {
        beginInsertRows(index(insertPos, 0), insertPos, insertPos);
        mPlaylist->insert(insertPos, item);
        indexesMoved.append(insertPos+i++);
        endInsertRows();
    }

    // Restore current index
    if(currentItem)
    {
        setCurrentIndex(mPlaylist->indexOf(currentItem));
    }

    endResetModel();
}
