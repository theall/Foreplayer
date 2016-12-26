#ifndef TPLAYLISTMODEL_H
#define TPLAYLISTMODEL_H

#include "abstractmodel.h"

class TPlaylistModel : public TAbstractModel
{
public:
    TPlaylistModel(QObject *parent = 0, TPlaylistItems *playlist = NULL);

    void setPlaylistCore(TPlaylistCore *core) Q_DECL_OVERRIDE;

    void setCurrentIndex(int index) Q_DECL_OVERRIDE;

    void add(QString name);
    void move(int from, int to);
    void remove(int index);

private:
    TPlaylistItems *mPlaylist;

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

};

#endif // TPLAYLISTMODEL_H
