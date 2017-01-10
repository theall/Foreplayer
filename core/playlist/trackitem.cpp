#include "trackitem.h"

#include "playlistdef.h"

TTrackItem::TTrackItem()
{

}

TTrackItem::~TTrackItem()
{

}

QJsonObject TTrackItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = displayName;
    object[K_INDEX] = index;
    object[K_FILE] = musicFile;
    object[K_DURATION] = duration;
    object[K_ENABLE] = enable;
    object[K_HIDDEN] = hidden;
    return object;
}

void TTrackItem::fromJson(QJsonObject object)
{
    displayName = object.value(K_NAME).toString(K_UNKNOWN);
    index = object.value(K_INDEX).toString();
    duration = object.value(K_DURATION).toInt();
    enable = object.value(K_ENABLE).toBool();
    hidden = object.value(K_HIDDEN).toBool();
}
