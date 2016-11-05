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

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    TMusicListDatas mMusicList;
};

#endif // TMUSICLISTMODEL_H
