#include "missionsmodel.h"

#define LABEL_TITLE         "Title"
#define LABEL_PROGRESS      "Progress"
#define LABEL_SOURCE        "Source"
#define LABEL_DESTINATION   "Destination"

enum ColumnIndex
{
    CI_TITLE=0,
    CI_PROGRESS,
    CI_SOURCE,
    CI_DESTINATION,
    CI_COUNT
};

TMissionsModel::TMissionsModel(QObject *parent, TExportMissions *exportMissions) :
    QStandardItemModel(parent)
  , mExportMissions(exportMissions)
{
    QStringList labels;
    labels << tr(LABEL_TITLE) << tr(LABEL_PROGRESS) << tr(LABEL_SOURCE) << tr(LABEL_DESTINATION);
    setColumnCount(labels.size());
    setHorizontalHeaderLabels(labels);
}

TMissionsModel::~TMissionsModel()
{

}

void TMissionsModel::update()
{
    emit layoutChanged();
}

QVariant TMissionsModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole && mExportMissions)
    {
        int row = index.row();
        int col = index.column();
        if(row>=0 && row<mExportMissions->size()-1)
        {
            TExportParam *param = (TExportParam*)mExportMissions->at(row)->data();
            switch (col) {
            case CI_TITLE:
                return QString::fromWCharArray(param->fileName);
                break;
            case CI_PROGRESS:
                return (float)param->progressCurrentFrames/param->progressTotalFrames;
                break;
            case CI_SOURCE:
                return QString::fromWCharArray(param->indexName);
                break;
            case CI_DESTINATION:
                return QString::fromWCharArray(param->outputPath);
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}

int TMissionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if(mExportMissions)
        return mExportMissions->size();
    return 0;
}

int TMissionsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return CI_COUNT;
}
