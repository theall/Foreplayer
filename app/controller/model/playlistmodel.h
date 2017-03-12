#ifndef TPLAYLISTMODEL_H
#define TPLAYLISTMODEL_H

#include "abstractmodel.h"

class TPlaylistModel : public TAbstractModel
{
public:
    TPlaylistModel(QObject *parent = 0);

    void setCurrentIndex(int index) Q_DECL_OVERRIDE;

    void add(QString name);
    void moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved);

    void remove(int index);
    void rename(int index, QString newName);
    void sortItems();

private:

public:
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TPLAYLISTMODEL_H
