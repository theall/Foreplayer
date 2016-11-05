#ifndef TTRACKLISTMODEL_H
#define TTRACKLISTMODEL_H

#include "abstractmodel.h"

struct TTrackListData
{
    QString name;
    QString index;
    QString album;
    QString duration;
};

typedef QList<TTrackListData*> TTrackListDatas;

class TTrackListModel : public TAbstractModel
{
public:
    TTrackListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    TTrackListDatas mTrackList;
};

#endif // TTRACKLISTMODEL_H
