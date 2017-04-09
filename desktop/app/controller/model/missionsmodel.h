#ifndef TMISSIONSMODEL_H
#define TMISSIONSMODEL_H

#include <QSharedMemory>
#include <QAbstractTableModel>

#include "utils.h"

typedef QList<QSharedMemory*> TExportMissions;

class TMissionsModel : public QAbstractTableModel
{
public:
    TMissionsModel(QObject *parent = 0, TExportMissions *exportMissions = NULL);
    ~TMissionsModel();

    void update();
    void addMission(QSharedMemory *mission);
    void addMissions(TExportMissions missions);

private:
    TExportMissions *mExportMissions;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
};

#endif // TMISSIONSMODEL_H
