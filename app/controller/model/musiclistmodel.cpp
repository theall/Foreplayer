#include "musiclistmodel.h"

TMusiclistModel::TMusiclistModel(QObject *parent) :
    TAbstractModel(parent)
  , mPlaylistItem(NULL)
{
}

void TMusiclistModel::setPlayListItem(TPlaylistItem *item)
{
    if(mPlaylistItem == item)
        return;

    mCurrentIndex = -1;
    mPlayingIndex = -1;
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

    return 4;
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

        if(column==1)
        {
            return QString("%1.").arg(index.row()+1);
        } else if (column==2) {
            return data->displayName();
        } else if (column==3) {
            return Utils::microSecToTimeStr(data->duration());
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
        TMusicItem *data = mPlaylistItem->musicItem(row);
        if(!data)
            return QVariant();

        return QString(tr("Title: %1\r\n"
                          "Duration: %2\r\n"
                          "Filename: %3\r\n"
                          "%4") \
                       .arg(data->displayName()) \
                       .arg(Utils::microSecToTimeStr(data->duration())) \
                       .arg(data->fileName())
                       .arg(data->additionalInfo()));
    }
    return TAbstractModel::data(index, role);
}

bool TMusiclistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole && index.column()==2)// The third column is title
    {
        int row = index.row();
        if(mPlaylistItem && row>-1 && row<mPlaylistItem->size())
            mPlaylistItem->musicItem(row)->setDisplayName(value.toString());
    }
    return TAbstractModel::setData(index, value, role);
}

void TMusiclistModel::setCurrentIndex(int index)
{
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
    TMusicItem *currentItem = mPlaylistItem->musicItem(mCurrentIndex);

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
    TMusicItem *currentItem = mPlaylistItem->musicItem(mCurrentIndex);

    beginResetModel();
    int i = 0;
    foreach (QString file, files) {
        TMusicItem *item = mPlaylistCore->parse(file);
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

    std::sort(indexes.begin(), indexes.end(), [=](int a, int b) {
        return a > b;
    });

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

void TMusiclistModel::update()
{
    emit dataChanged(QModelIndex(), QModelIndex());
}

TPlaylistItem *TMusiclistModel::playlistItem()
{
    return mPlaylistItem;
}
