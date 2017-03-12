#include "playlistmodel.h"

TPlaylistModel::TPlaylistModel(QObject *parent) :
    TAbstractModel(parent)
{

}

void TPlaylistModel::setCurrentIndex(int index)
{
    TAbstractModel::setCurrentIndex(index);
}

void TPlaylistModel::add(QString name)
{
    if(!mPlaylistCore)
        return;

    int index = mPlaylistCore->size();
    beginInsertRows(QModelIndex(), index, index);
    mPlaylistCore->insert(name);
    endInsertRows();
}

void TPlaylistModel::remove(int index)
{
    if(!mPlaylistCore)
        return;

    if(index<0 || index>=mPlaylistCore->size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    mPlaylistCore->remove(index);
    endRemoveRows();

    if(index < mCurrentIndex)
    {
        mCurrentIndex--;

        if(mCurrentIndex < 0)
            mCurrentIndex = 0;
    }

    int newSize = mPlaylistCore->size();
    if(mCurrentIndex >= newSize)
        mCurrentIndex = newSize - 1;
}

void TPlaylistModel::rename(int index, QString newName)
{
    if(!mPlaylistCore)
        return;

    if(index<0 || index>=mPlaylistCore->size())
        return;

    TPlaylistItem *item = mPlaylistCore->playlistItem(index);
    if(item)
        item->setDisplayName(newName);
}

void TPlaylistModel::sortItems()
{
    if(!mPlaylistCore)
        return;

    mPlaylistCore->sort();
}

int TPlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mPlaylistCore)
        return mPlaylistCore->size();

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
        if(mPlaylistCore && row>-1 && row<mPlaylistCore->size())
            return mPlaylistCore->playlistItem(row)->name();
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
        if(mPlaylistCore && row>-1 && row<mPlaylistCore->size())
            mPlaylistCore->playlistItem(row)->setDisplayName(value.toString());
    }
    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TPlaylistModel::flags(const QModelIndex &index) const
{
    return TAbstractModel::flags(index)|Qt::ItemIsEditable;
}

void TPlaylistModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    if(!mPlaylistCore)
        return;

    int listSize = mPlaylistCore->size();
    if(indexes.count()<=0 || listSize<=0)
        return;

    if(pos < 0)
        pos = 0;
    if(pos > listSize)
        pos = listSize;

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b) {
        return a > b;
    });
    TPlaylistItems items;

    // Find and record current index
    TPlaylistItem *currentItem = mPlaylistCore->currentPlaylistItem();

    beginResetModel();
    foreach (int i, indexes) {
        if(i<0 || i>=listSize)
            continue;
        beginRemoveRows(index(i, 0), i, i);
        items.append(mPlaylistCore->takeAt(i));
        endRemoveRows();
        if(i < pos)
            pos--;
    }
    int i = 0;
    foreach (TPlaylistItem *item, items) {
        beginInsertRows(index(pos, 0), pos, pos);
        mPlaylistCore->insert(pos, item);
        indexesMoved.append(pos+i++);
        endInsertRows();
    }

    // Restore current index
    if(currentItem)
    {
        setCurrentIndex(mPlaylistCore->indexOf(currentItem));
    }

    endResetModel();
}
