#ifndef TMUSICITEM_H
#define TMUSICITEM_H

#include "trackitem.h"

enum SortMethod
{
    SM_TITLE_ASC,
    SM_TITLE_DES,
    SM_LENGTH_ASC,
    SM_LENGTH_DES,
    SM_DURATION_ASC,
    SM_DURATION_DES,
    SM_INDEX_ASC,
    SM_INDEX_DES,
    SM_FILE_ASC,
    SM_FILE_DES,
    SM_RANDOM,
    SM_REVERSE
};

class TMusicItem
{
public:
    TMusicItem();
    ~TMusicItem();

    QJsonObject toJson();

    void fromJson(QJsonObject object);

    void fromTrackItem(TTrackItem *trackItem);

    void sort(SortMethod mode);

    QString displayName();
    void setDisplayName(QString displayName);

    QString originalName();
    void setOriginalName(QString name);

    QString fileName();
    void setFileName(QString fileName);

    QString system();
    void setSystem(QString system);

    QString artist();
    void setArtist(QString artist);

    QString year();
    void setYear(QString year);

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
    void addTrackItem(TTrackItem *trackItem, int pos=-1);

    bool isModified();
    void setModified(bool modified=true);

    int size();
    void clear();

    static QString mimeType() { return "forplayer/musicitem"; }

private:
    QString mDisplayName;
    QString mOriginalName;
    QString mFileName;
    QString mSystem;
    QString mArtist;
    QString mYear;
    QString mAdditionalInfo;
    int mDuration;
    int mFileSize;
    bool mModified;
    QDateTime mLastParsed;
    TTrackItems mTrackItems;
};

typedef QList<TMusicItem*> TMusicItems;

#endif // TMUSICITEM_H
