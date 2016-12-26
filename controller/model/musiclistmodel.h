#ifndef TMUSICLISTMODEL_H
#define TMUSICLISTMODEL_H

#include "abstractmodel.h"

struct TMusicListData
{
    QString title;
    QString artist;
    QString album;
    QString format;
    QString duration;
    QString fileName;
};

typedef QList<TMusicListData*> TMusicListDatas;

class TMusiclistModel : public TAbstractModel
{
public:
    TMusiclistModel(QObject *parent = 0);

private:
    TMusicListDatas mMusicList;

public:
    // TAbstractModel interface
    void setCurrentIndex(int index) Q_DECL_OVERRIDE;
    void setPlaylistCore(TPlaylistCore *core) Q_DECL_OVERRIDE;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTMODEL_H
