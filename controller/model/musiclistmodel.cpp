#include "musiclistmodel.h"

TMusiclistModel::TMusiclistModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}



int TMusiclistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 0;
}

int TMusiclistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 3;
}

QVariant TMusiclistModel::data(const QModelIndex &index, int role) const
{    
    return QAbstractTableModel::data(index, role);
}
