#ifndef TTRACKLISTMODEL_H
#define TTRACKLISTMODEL_H

#include "pch.h"

class TTracklistModel : public QAbstractTableModel
{
public:
    TTracklistModel(QObject *parent = 0);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};

#endif // TTRACKLISTMODEL_H
