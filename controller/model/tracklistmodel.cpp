#include "tracklistmodel.h"

TTracklistModel::TTracklistModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

int TTracklistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
}

int TTracklistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant TTracklistModel::data(const QModelIndex &index, int role) const
{
    return QAbstractTableModel::data(index, role);
}
