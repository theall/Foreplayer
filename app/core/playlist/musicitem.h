#ifndef TMUSICITEM_H
#define TMUSICITEM_H

#include "trackitem.h"
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

class TMusicItem
{
public:
    TMusicItem();
    ~TMusicItem();

    void *plugin;
    QJsonObject toJson();

    void fromJson(QJsonObject object);

    void sort(SortMode mode);

    QString displayName();
    void setDisplayName(QString displayName);

    QString fileName();
    void setFileName(QString fileName);

    QString additionalInfo();
    void setAdditionalInfo(QString info);

    int duration();
    void setDuration(int duration);

    int fileSize();
    void setFileSize(int fileSize);

    QDateTime lastParsed();
    void setLastParsed(QDateTime lastParsed);

    TTrackItems *trackItems();
    TTrackItem *trackItem(int index);

    void setModified();

    int size();
    void clear();

private:
    QString mDisplayName;
    QString mFileName;
    QString mAdditionalInfo;
    int mDuration;
    int mFileSize;
    bool mModified;
    QDateTime mLastParsed;
    TTrackItems mTrackItems;
};

typedef QList<TMusicItem*> TMusicItems;

#endif // TMUSICITEM_H
