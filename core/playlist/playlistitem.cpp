#include "playlistitem.h"

#define PL_VERSION 1
#define K_NAME "Name"
#define K_DEFAULT "Default"
#define K_UNKNOWN "Unknown"
#define K_VERSION "Version"
#define K_PLAYLIST "Playlist"
#define K_MUSICLIST "Musiclist"
#define K_TRACKLIST "Tracklist"
#define K_GENERATOR "Generator"
#define K_FILE "File"
#define K_DURATION "Duration"
#define K_SIZE "FileSize"
#define K_LAST_PARSED "LastParsed"
#define K_INDEX "Index"
#define K_ENABLE "Enable"

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
    return object;
}

void TTrackItem::fromJson(QJsonObject object)
{
    displayName = object.value(K_NAME).toString(K_UNKNOWN);
    index = object.value(K_INDEX).toString();
    duration = object.value(K_DURATION).toInt();
    enable = object.value(K_ENABLE).toBool();
}

TMusicItem::TMusicItem()
{
    duration = 0;
    fileSize = 0;
}

TMusicItem::~TMusicItem()
{
    foreach (TTrackItem *item, tracks) {
        delete item;
    }
    tracks.clear();
}

QJsonObject TMusicItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = displayName;
    object[K_FILE] = fileName;
    object[K_DURATION] = duration;
    object[K_SIZE] = fileSize;
    object[K_LAST_PARSED] = lastParsed.toString();

    QJsonArray tracksArray;
    foreach (TTrackItem *trackItem, tracks) {
        tracksArray.append(trackItem->toJson());
    }

    object[K_TRACKLIST] = tracksArray;
    return object;
}

void TMusicItem::fromJson(QJsonObject object)
{
    displayName = object.value(K_NAME).toString(K_UNKNOWN);
    fileName = object.value(K_FILE).toString();
    fileSize = object.value(K_SIZE).toInt();
    duration = object.value(K_DURATION).toInt();
    lastParsed = QDateTime::fromString(object.value(K_LAST_PARSED).toString());
    QJsonArray trackArray = object.value(K_TRACKLIST).toArray();
    foreach (QJsonValue trackValue, trackArray) {
        QJsonObject trackObject = trackValue.toObject();
        TTrackItem *trackItem = new TTrackItem;
        trackItem->musicFile = fileName;
        trackItem->fromJson(trackObject);
        tracks.append(trackItem);
    }
}

void TMusicItem::sort(SortMode mode)
{
    if(mode==TITLE_ASC)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() < b->displayName.toLower();
        });
    else if (mode==TITLE_DES)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() > b->displayName.toLower();
        });
    else if (mode==LENGTH_ASC)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() < b->displayName.count();
        });
    else if (mode==LENGTH_DES)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() > b->displayName.count();
        });
    else if (mode==DURATION_ASC)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration < b->duration;
        });
    else if (mode==DURATION_DES)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration > b->duration;
        });
    else if (mode==INDEX_ASC)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index < b->index;
        });
    else if (mode==INDEX_DES)
        std::sort(tracks.begin(), tracks.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index > b->index;
        });
    else if (mode==RANDOM)
    {
        int n = tracks.size()-2;
        for(int i=0;i<tracks.size();i++)
        {
            int index = rand()*n/RAND_MAX;
            TTrackItem *item = tracks.takeAt(index);
            tracks.append(item);
        }
    }
}

TPlaylistItem::TPlaylistItem()
{
    modified = false;
}

TPlaylistItem::~TPlaylistItem()
{
    foreach (TMusicItem *item, musics) {
        delete item;
    }
    musics.clear();
}

QJsonObject TPlaylistItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = displayName;
    object[K_FILE] = fileName;
    object[K_VERSION] = version;

    QJsonArray musicsArray;
    foreach (TMusicItem *musicItem, musics) {
        musicsArray.append(musicItem->toJson());
    }
    object[K_MUSICLIST] = musicsArray;
    return object;
}

void TPlaylistItem::fromJson(QJsonObject object)
{
    displayName = object.value(K_NAME).toString(K_DEFAULT);
    version = object.value(K_VERSION).toInt(PL_VERSION);
    QJsonArray musicArray = object.value(K_MUSICLIST).toArray();
    foreach (QJsonValue musicValue, musicArray) {
        QJsonObject musicObject = musicValue.toObject();
        TMusicItem *musicItem = new TMusicItem;
        musicItem->fromJson(musicObject);
        musics.append(musicItem);
    }
    musicCount = musicArray.size();
}

void TPlaylistItem::sort(SortMode mode)
{
    if(mode==TITLE_ASC)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName.toLower() < b->displayName.toLower();
        });
    else if (mode==TITLE_DES)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName.toLower() > b->displayName.toLower();
        });
    else if (mode==LENGTH_ASC)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName.count() < b->displayName.count();
        });
    else if (mode==LENGTH_DES)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName.count() > b->displayName.count();
        });
    else if (mode==DURATION_ASC)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration < b->duration;
        });
    else if (mode==DURATION_DES)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration > b->duration;
        });
    else if (mode==FILE_ASC)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName < b->fileName;
        });
    else if (mode==FILE_DES)
        std::sort(musics.begin(), musics.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName > b->fileName;
        });
    else if (mode==RANDOM)
    {
        int n = musics.size()-2;
        for(int i=0;i<musics.size();i++)
        {
            int index = rand()*n/RAND_MAX;
            TMusicItem *item = musics.takeAt(index);
            musics.append(item);
        }
    }
}

void TPlaylistItem::clear()
{
    foreach (TMusicItem *item, musics) {
        delete item;
    }
    musics.clear();
}
