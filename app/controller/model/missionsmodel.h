#ifndef TMISSIONSMODEL_H
#define TMISSIONSMODEL_H

#include <QSharedMemory>
#include <QStandardItemModel>

#include "utils.h"

typedef QList<QSharedMemory*> TExportMissions;

class TMissionsModel : public QStandardItemModel
{
public:
    TMissionsModel(QObject *parent = 0, TExportMissions *exportMissions = NULL);
    ~TMissionsModel();

    void update();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    TExportMissions *mExportMissions;

    // QStandardItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
};

#endif // TMISSIONSMODEL_H
