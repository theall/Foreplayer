#ifndef TPLAYLISTMODEL_H
#define TPLAYLISTMODEL_H

#include "abstractmodel.h"

class TPlaylistModel : public TAbstractModel
{
public:
    TPlaylistModel(QObject *parent = 0);

    void addPlaylist(QString name);
    void removePlaylist(int index);

private:
    QStringList mPlaylist;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTMODEL_H
