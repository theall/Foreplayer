#ifndef TPLAYLISTITEM_H
#define TPLAYLISTITEM_H

#include "pch.h"

enum SortMode
{
    TITLE_ASC,
    TITLE_DES,
    LENGTH_ASC,
    LENGTH_DES,
    DURATION_ASC,
    DURATION_DES,
    INDEX_ASC,
    INDEX_DES,
    FILE_ASC,
    FILE_DES,
    RANDOM
};

class TTrackItem
{
public:
    QString displayName;
    QString index;
    QString musicFile;
    int duration;
    bool enable;

    TTrackItem();

    ~TTrackItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);
};
typedef QList<TTrackItem*> TTrackItems;

class TMusicItem
{
public:
    QString displayName;
    QString fileName;
    int duration;
    int fileSize;
    QDateTime lastParsed;
    TTrackItems tracks;

    TMusicItem();

    ~TMusicItem();

    QJsonObject toJson();

    void fromJson(QJsonObject object);

    void sort(SortMode mode);
};

typedef QList<TMusicItem*> TMusicItems;

class TPlaylistItem
{
public:
    TMusicItems musics;
    QString fileName;
    bool modified;
    int version;

    TPlaylistItem();
    ~TPlaylistItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);

    inline QString name() { return mDisplayName; }
    void setDisplayName(QString newName);
    void sort(SortMode mode);

    void clear();

private:
    QString mDisplayName;
};

typedef QList<TPlaylistItem*> TPlaylistItems;

#endif // TPLAYLISTITEM_H
