#include "musiclistmodel.h"

TMusiclistModel::TMusiclistModel(QObject *parent) :
    TAbstractModel(parent)
{
    for(int i=0;i<10;i++)
    {
        TMusicListData *data = new TMusicListData;
        data->title = "fulkfour"+QString::number(i);
        data->album = "beyond";
        data->artist = "beyond";
        data->duration = "2:22";
        data->fileName = "z:\a.mp3";
        data->format = "MP3 44KHZ 128K(VBR)";

        mMusicList.append(data);
    }
}

int TMusiclistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return mMusicList.size();
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
        if(row<0 || row>=mMusicList.size())
            return QVariant();

        TMusicListData *data = mMusicList[row];

        if(column==0)
        {
            return QString(" %1.").arg(index.row()+1);
        } else if (column==1) {
            return data->artist + "-" + data->title;
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
        if(row<0 || row>=mMusicList.size())
            return QVariant();

        TMusicListData *data = mMusicList[row];
        return QString(tr("Title: %1\r\nArtist: %2\r\nAlbum: %3\r\nFormat: %4\r\nDuration: %5\r\nFilename: %6") \
                       .arg(data->title) \
                       .arg(data->artist) \
                       .arg(data->album) \
                       .arg(data->format) \
                       .arg(data->duration) \
                       .arg(data->fileName));
    }
    return TAbstractModel::data(index, role);
}
