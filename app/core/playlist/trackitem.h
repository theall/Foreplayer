#ifndef TTRACKITEM_H
#define TTRACKITEM_H

#include "pch.h"

class TTrackItem
{
public:
    QString displayName;
    QString index;
    QString additionalInfo;
    QString fileName;
    int duration;
    bool enable;
    bool hidden;
    TTrackItem();

    ~TTrackItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);
};
typedef QList<TTrackItem*> TTrackItems;

#endif // TTRACKITEM_H
