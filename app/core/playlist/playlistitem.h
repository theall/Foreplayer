#ifndef TPLAYLISTITEM_H
#define TPLAYLISTITEM_H

#include "musicitem.h"

class TPlaylistItem
{
public:
    TPlaylistItem(QString fileName = QString());
    ~TPlaylistItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);

    int size();
    TMusicItem *takeAt(int index);
    void insert(int pos, TMusicItem *item);
    bool remove(int index);
    QList<int> removeRedundant();
    QList<int> removeErrors();
    void rename(int index, QString newName);
    void sort(SortMode mode = TITLE_ASC);
    int indexOf(TMusicItem *item);

    QString name() { return mDisplayName; }
    void setDisplayName(QString newName);

    TMusicItem *currentItem();
    int currentIndex() { return mCurrentIndex; }
    void setCurrentIndex(int index);

    TMusicItem *musicItem(int index);

    QString fileName() { return mFileName; }
    void setFileName(QString fileName);

    bool isModified() { return mModified; }

    int version() { return mVersion; }

    void save();
    void clear();

private:
    QString mDisplayName;
    TMusicItems mMusicItems;
    QString mFileName;
    bool mModified;
    int mVersion;
    int mCurrentIndex;
};

typedef QList<TPlaylistItem*> TPlaylistItems;

#endif // TPLAYLISTITEM_H
