#ifndef TTRACKITEM_H
#define TTRACKITEM_H

#include <string>
#include <QJsonObject>

class TTrackItem
{
public:
    std::wstring displayName;
    std::wstring originalName;
    std::wstring index;
    std::wstring indexName;
    std::wstring additionalInfo;
    std::wstring fileName;
    std::wstring system;
    std::wstring artist;
    std::wstring year;
    int duration;
    int originalDuration;
    bool enable;
    bool hidden;
    TTrackItem();

    ~TTrackItem();

    QJsonObject toJson();
    void fromJson(QJsonObject object);

    static std::string mimeType() { return "forplayer/trackitem"; }
};
typedef QList<TTrackItem*> TTrackItems;

#endif // TTRACKITEM_H
