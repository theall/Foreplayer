#include "musiclistmodel.h"

#define RecordCurrentItem MusicItem currentItem = mCore->getMusicItem(mPlaylistItem, mCurrentIndex)
#define RestoreCurrentItem \
if(currentItem)\
    setCurrentIndex(mCore->getMusicItemIndex(mPlaylistItem, currentItem))

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
        return mCore->getMusicItemCount(mPlaylistItem);

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
                align = Qt::AlignRight;;
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
    if(index.column() == 2)// Title column is editable
        _flags |= Qt::ItemIsEditable;
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

    indexesMoved = mCore->moveMusicItems(mPlaylistItem, indexes, pos);

    // Restore current index
    RestoreCurrentItem;

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

    for(MusicItem musicItem : musicItems) {
        if(musicItem)
        {
            beginInsertRows(index(pos, 0), pos, pos);
            int posr = mCore->insertMusicItem(mPlaylistItem, pos, musicItem);
            newIndexes.append(posr);
            endInsertRows();
        }
    }
    endResetModel();

    // Restore current index
    RestoreCurrentItem;
}

void TMusiclistModel::insertItems(int pos, MusicItems musicItems)
{
    QList<int> newIndexes;
    insertItems(pos, musicItems, newIndexes);
}

void TMusiclistModel::removeSelections(QList<int> indexes)
{
    if(!mPlaylistItem)
        return;

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b) {
        return a > b;
    });

    for(int i : indexes) {
        beginRemoveRows(QModelIndex(), i, i);
        mCore->removeMusicItem(mPlaylistItem, i);
        endRemoveRows();
    }
}

QList<int> TMusiclistModel::removeRedundant()
{
    QList<int> rowsRemoved;

    if(!mPlaylistItem)
        return rowsRemoved;

    rowsRemoved = mCore->removeRedundant(mPlaylistItem);

    for(int i : rowsRemoved) {
        beginRemoveRows(QModelIndex(), i, i);
        endRemoveRows();
    }

    return rowsRemoved;
}

QList<int> TMusiclistModel::removeErrors()
{
    QList<int> rowsRemoved;

    if(!mPlaylistItem)
        return rowsRemoved;

    rowsRemoved = mCore->removeErrors(mPlaylistItem);

    for(int i : rowsRemoved) {
        beginRemoveRows(QModelIndex(), i, i);
        endRemoveRows();
    }

    return rowsRemoved;
}

void TMusiclistModel::removeAll()
{
    if(!mPlaylistItem)
        return;

    beginResetModel();
    mCore->clear(mPlaylistItem);
    endResetModel();
}

void TMusiclistModel::update()
{
    emit dataChanged(QModelIndex(), QModelIndex());
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

    beginResetModel();
    mCore->sort(mPlaylistItem, sm);
    endResetModel();

    RestoreCurrentItem;
}
