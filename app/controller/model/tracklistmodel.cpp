#include "tracklistmodel.h"

int strToMSecs(QString s)
{
    QStringList sl = s.split(":");
    int length = sl.size();
    int hour = 0;
    int minute = 0;
    int sec = 0;
    if(length > 0)
    {
        sec = sl[length-1].toInt();
        if(length > 1)
        {
            minute = sl[length-2].toInt();
            if(length > 2)
                hour = sl[length-3].toInt();
        }
    }
    return (hour*3600+minute*60+sec)*1000;
}

TTrackListModel::TTrackListModel(QObject *parent) :
    TAbstractModel(parent)
  , mMusicItem(NULL)
{

}

void TTrackListModel::moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved)
{
    Q_UNUSED(indexes);
    Q_UNUSED(pos);
    Q_UNUSED(indexesMoved);
}

void TTrackListModel::setMusicItem(TMusicItem *item)
{
    if(mMusicItem==item)
        return;

    mCurrentIndex = -1;
    mPlayingIndex = -1;
    beginResetModel();
    mMusicItem = item;
    endResetModel();

    emit requestAdjustColumnWidth();
}

TMusicItem *TTrackListModel::musicItem()
{
    return mMusicItem;
}

int TTrackListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(mMusicItem)
        return mMusicItem->size();

    return 0;
}

int TTrackListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 4;
}

QVariant TTrackListModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole || role==Qt::EditRole)
    {
        int column = index.column();
        int row = index.row();
        if(row<0 || row>=mMusicItem->size())
            return QVariant();

        TTrackItem *item = mMusicItem->trackItem(row);

        if(column==1)
        {
            return QString::number(index.row()+1) + ".";
        } else if (column==2) {
            return item->displayName;
        } else if (column==3) {
            return Utils::microSecToTimeStr(item->duration);
        }
    } else if (role==Qt::TextAlignmentRole) {
        Qt::Alignment align;
        if (index.column()==2) {
            align = Qt::AlignLeft;
        } else {
            align = Qt::AlignRight;;
        }
        return QVariant(align|Qt::AlignVCenter);
    } else if (role==Qt::TextColorRole) {
        if(mMusicItem && !mMusicItem->trackItem(index.row())->enable)
        {
            return QColor("#3c3c3c");
        }
    } else if (role==Qt::ToolTipRole) {
        int row = index.row();
        if(row<0 || row>=mMusicItem->size())
            return QVariant();

        TTrackItem *item = mMusicItem->trackItem(row);
        QString displayIndex = item->indexName;
        if(displayIndex.isEmpty())
            displayIndex = item->index;
        return QString(tr("Index: %1\r\nName: %2\r\nDuration: %3\r\n%4") \
                       .arg(displayIndex) \
                       .arg(item->displayName) \
                       .arg(Utils::microSecToTimeStr(item->duration)) \
                       .arg(item->additionalInfo));
    }
    return TAbstractModel::data(index, role);
}

bool TTrackListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)// The third column is title
    {
        int col = index.column();
        int row = index.row();
        if(mMusicItem && row>-1 && row<mMusicItem->size())
        {
            TTrackItem *trackItem = mMusicItem->trackItem(row);
            if(trackItem)
            {
                if(col==2)
                {
                    // Title
                    QString newName = value.toString();
                    if(!newName.isEmpty() && newName != trackItem->displayName)
                    {
                        trackItem->displayName = newName;
                        mMusicItem->setModified();
                    }
                } else if (col==3) {
                    // Duration
                    int newDuration = strToMSecs(value.toString());
                    if(newDuration>=0 && newDuration != trackItem->duration)
                    {
                        int diff = newDuration - trackItem->duration;
                        trackItem->duration = newDuration;
                        mMusicItem->setDuration(mMusicItem->duration()+diff);
                        emit requestAdjustColumnWidth();
                    }
                }
            }
        }
    }
    return TAbstractModel::setData(index, value, role);
}

Qt::ItemFlags TTrackListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags _flags = TAbstractModel::flags(index);
    if(index.column()>1)// Title and length columns are editable
        _flags |= Qt::ItemIsEditable;

    return _flags;
}

void TTrackListModel::setCurrentIndex(int index)
{
    TAbstractModel::setCurrentIndex(index);
}

void TTrackListModel::setPlaylistCore(TPlaylistCore *core)
{
    TAbstractModel::setPlaylistCore(core);
}
