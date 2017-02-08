#include "trackitem.h"

#include "playlistdef.h"

TTrackItem::TTrackItem()
{
    enable = true;
    hidden = false;
}

TTrackItem::~TTrackItem()
{

}

QJsonObject TTrackItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = displayName;
    object[K_INNER_NAME] = fileName;
    object[K_INDEX] = index;
    object[K_INDEX_NAME] = indexName;
    object[K_ADDITIONAL] = additionalInfo;
    object[K_FILE] = fileName;
    object[K_DURATION] = duration;
    object[K_ENABLE] = enable;
    object[K_HIDDEN] = hidden;
    return object;
}

void TTrackItem::fromJson(QJsonObject object)
{
    displayName = object.value(K_NAME).toString(K_UNKNOWN);
    fileName = object.value(K_INNER_NAME).toString(K_UNKNOWN);
    index = object.value(K_INDEX).toString();
    indexName = object.value(K_INDEX_NAME).toString();
    additionalInfo = object.value(K_ADDITIONAL).toString();
    duration = object.value(K_DURATION).toInt();
    enable = object.value(K_ENABLE).toBool();
    hidden = object.value(K_HIDDEN).toBool();
}
