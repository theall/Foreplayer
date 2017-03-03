#ifndef TMUSICLISTMODEL_H
#define TMUSICLISTMODEL_H

#include "abstractmodel.h"

class TMusiclistModel : public TAbstractModel
{
public:
    TMusiclistModel(QObject *parent = 0);

    void setPlayListItem(TPlaylistItem *item);
    void moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved);
    void insertFiles(QStringList files, int pos, QList<int> &newIndexes);
    void insertItems(int pos, TMusicItems musicItems, QList<int> &newIndexes);
    void insertItems(int pos, TMusicItems musicItems);
    void removeSelections(QList<int> indexes);
    QList<int> removeRedundant();
    QList<int> removeErrors();
    void removeAll();
    void update();

    TPlaylistItem *playlistItem();

private:
    TPlaylistItem *mPlaylistItem;

public:
    // TAbstractModel interface
    void setCurrentIndex(int index) Q_DECL_OVERRIDE;
    void setPlaylistCore(TPlaylistCore *core) Q_DECL_OVERRIDE;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TMUSICLISTMODEL_H
