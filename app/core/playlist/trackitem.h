#ifndef TTRACKITEM_H
#define TTRACKITEM_H

#include "pch.h"

class TTrackItem
{
public:
    QString displayName;
    QString originalName;
    QString index;
    QString indexName;
    QString additionalInfo;
    QString fileName;
    QString system;
    QString artist;
    QString year;
    int duration;
    int originalDuration;
    bool enable;
    bool hidden;
    TTrackItem();

    ~TTrackItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);

    static QString mimeType() { return "forplayer/trackitem"; }
};
typedef QList<TTrackItem*> TTrackItems;

#endif // TTRACKITEM_H
