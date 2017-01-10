#include "musiclistmodel.h"

TMusiclistModel::TMusiclistModel(QObject *parent) :
    TAbstractModel(parent)
  , mPlaylistItem(NULL)
{
}

void TMusiclistModel::setPlayListItem(TPlaylistItem *item)
{
    beginResetModel();
    mPlaylistItem = item;
    endResetModel();
}

int TMusiclistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(mPlaylistItem)
        return mPlaylistItem->size();
    return 0;
}

int TMusiclistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant TMusiclistModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int column = index.column();
        int row = index.row();
        TMusicItem *data = mPlaylistItem->musicItem(row);
        if(!data)
            return QVariant();

        if(column==0)
        {
            return QString(" %1.").arg(index.row()+1);
        } else if (column==1) {
            return data->artist() + "-" + data->displayName();
        } else if (column==2) {
            return data->duration();
        }
    } else if (role==Qt::TextAlignmentRole) {
        Qt::Alignment align;
        if (index.column()==1) {
            align = Qt::AlignLeft;
        } else {
            align = Qt::AlignRight;;
        }
        return QVariant(align|Qt::AlignVCenter);
    } else if (role==Qt::ToolTipRole) {
        int row = index.row();
        TMusicItem *data = mPlaylistItem->musicItem(row);
        if(!data)
            return QVariant();

        return QString(tr("Title: %1\r\n"
                          "Artist: %2\r\n"
                          "Album: %3\r\n"
                          "Format: %4\r\n"
                          "Duration: %5\r\n"
                          "Filename: %6") \
                       .arg(data->displayName()) \
                       .arg(data->artist()) \
                       .arg(data->album()) \
                       .arg(data->format()) \
                       .arg(data->duration()) \
                       .arg(data->fileName()));
    }
    return TAbstractModel::data(index, role);
}

void TMusiclistModel::setCurrentIndex(int index)
{
    if(mPlaylistItem)
        mPlaylistItem->setCurrentIndex(index);

    TAbstractModel::setCurrentIndex(index);
}

void TMusiclistModel::setPlaylistCore(TPlaylistCore *core)
{
    TAbstractModel::setPlaylistCore(core);
}

void TMusiclistModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    if(!mPlaylistCore || !mPlaylistItem)
        return;

    int musicsCount = mPlaylistItem->size();
    if(indexes.count()<=0 || musicsCount<=0)
        return;

    if(pos < 0)
        pos = 0;
    if(pos > musicsCount)
        pos = musicsCount;

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b) {
        return a > b;
    });
    TMusicItems items;

    // Record current index
    TMusicItem *currentItem = mPlaylistItem->currentItem();

    beginResetModel();
    foreach (int i, indexes) {
        if(i<0 || i>=musicsCount)
            continue;
        beginRemoveRows(index(i, -1), i, i);
        items.append(mPlaylistItem->takeAt(i));
        endRemoveRows();
        if(i < pos)
            pos--;
    }
    int i = 0;
    foreach (TMusicItem *item, items) {
        beginInsertRows(index(pos, 0), pos, pos);
        mPlaylistItem->insert(pos, item);
        indexesMoved.append(pos+i++);
        endInsertRows();
    }

    // Restore current index
    if(currentItem)
    {
        setCurrentIndex(mPlaylistItem->indexOf(currentItem));
    }

    endResetModel();
}

void TMusiclistModel::insertFiles(QStringList files, int pos, QList<int> &newIndexes)
{
    if(!mPlaylistCore || !mPlaylistItem)
        return;

    if(files.count()<=0)
        return;

    int musicsCount = mPlaylistItem->size();

    if(pos < 0)
        pos = 0;

    if(pos > musicsCount)
        pos = musicsCount;

    // Record current item
    TMusicItem *currentItem = mPlaylistItem->currentItem();

    beginResetModel();
    int i = 0;
    foreach (QString file, files) {
        TMusicItem *item = new TMusicItem;
        item->setDisplayName(file);
        item->setFileName(file);
        beginInsertRows(index(pos, 0), pos, pos);
        mPlaylistItem->insert(pos, item);
        newIndexes.append(pos+i++);
        endInsertRows();
    }

    // Restore current index
    if(currentItem)
    {
        setCurrentIndex(mPlaylistItem->indexOf(currentItem));
    }

    endResetModel();
}

void TMusiclistModel::removeSelections(QList<int> indexes)
{
    if(!mPlaylistItem)
        return;

    beginResetModel();
    foreach (int i, indexes) {
        mPlaylistItem->remove(i);
    }
    endResetModel();
}

QList<int> TMusiclistModel::removeRedundant()
{
    QList<int> rowsRemoved;

    if(!mPlaylistItem)
        return rowsRemoved;

    rowsRemoved = mPlaylistItem->removeRedundant();

    foreach (int i, rowsRemoved) {
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

    rowsRemoved = mPlaylistItem->removeErrors();

    foreach (int i, rowsRemoved) {
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
    mPlaylistItem->clear();
    endResetModel();
}
