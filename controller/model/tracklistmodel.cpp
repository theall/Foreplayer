#include "tracklistmodel.h"

TTrackListModel::TTrackListModel(QObject *parent) :
    TAbstractModel(parent)
{
    for(int i=0;i<10;i++)
    {
        TTrackListData *data = new TTrackListData;
        data->name = "Wild care";
        data->index = QString::number(i+1);
        data->duration = "4:23";
        data->album = "dino 1993 capcom system";
        mTrackList.append(data);
    }
}

int TTrackListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mTrackList.size();
}

int TTrackListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant TTrackListModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int column = index.column();
        int row = index.row();
        if(row<0 || row>=mTrackList.size())
            return QVariant();

        TTrackListData *data = mTrackList[row];

        if(column==0)
        {
            return QString::number(index.row()+1) + ".";
        } else if (column==1) {
            return data->index + "-" + data->name;
        } else if (column==2) {
            return data->duration;
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
        if(row<0 || row>=mTrackList.size())
            return QVariant();

        TTrackListData *data = mTrackList[row];
        return QString(tr("Name: %1-%2\r\nAlbum: %3\r\nDuration: %4") \
                       .arg(data->index) \
                       .arg(data->name) \
                       .arg(data->album) \
                       .arg(data->duration));
    }
    return TAbstractModel::data(index, role);
}

void TTrackListModel::setCurrentIndex(int index)
{
    if(mPlaylistCore)
        mPlaylistCore->setCurrentIndex(TrackList, index);

    TAbstractModel::setCurrentIndex(index);
}

void TTrackListModel::setPlaylistCore(TPlaylistCore *core)
{
    TAbstractModel::setPlaylistCore(core);
}
