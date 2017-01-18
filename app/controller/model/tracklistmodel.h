#ifndef TTRACKLISTMODEL_H
#define TTRACKLISTMODEL_H

#include "abstractmodel.h"

class TTrackListModel : public TAbstractModel
{
public:
    TTrackListModel(QObject *parent = 0);

    void moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved);
    void setMusicItem(TMusicItem *item);

private:
    TMusicItem *mMusicItem;

public:
    // TAbstractModel interface
    void setCurrentIndex(int index) Q_DECL_OVERRIDE;
    void setPlaylistCore(TPlaylistCore *core) Q_DECL_OVERRIDE;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // TTRACKLISTMODEL_H