#ifndef TMUSICLISTMODEL_H
#define TMUSICLISTMODEL_H

#include "pch.h"

class TMusiclistModel : public QAbstractTableModel
{
public:
    TMusiclistModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTMODEL_H
