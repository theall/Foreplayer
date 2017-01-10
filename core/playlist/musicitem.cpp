#include "musicitem.h"

#include "playlistdef.h"

TMusicItem::TMusicItem() :
    mDuration(0)
    , mFileSize(0)
    , mModified(false)
{

}

TMusicItem::~TMusicItem()
{
    clear();
}

QJsonObject TMusicItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = mDisplayName;
    object[K_FILE] = mFileName;
    object[K_DURATION] = mDuration;
    object[K_ARTIST] = mArtist;
    object[K_ALBUM] = mAlbum;
    object[K_FORMAT] = mFormat;
    object[K_SIZE] = mFileSize;
    object[K_LAST_PARSED] = mLastParsed.toString();

    QJsonArray tracksArray;
    foreach (TTrackItem *trackItem, mTrackItems) {
        tracksArray.append(trackItem->toJson());
    }

    object[K_TRACKLIST] = tracksArray;
    return object;
}

void TMusicItem::fromJson(QJsonObject object)
{
    mDisplayName = object.value(K_NAME).toString(K_UNKNOWN);
    mFileName = object.value(K_FILE).toString();
    mFileSize = object.value(K_SIZE).toInt();
    mDuration = object.value(K_DURATION).toInt();
    mArtist = object.value(K_ARTIST).toString();
    mAlbum = object.value(K_ALBUM).toString();
    mFormat = object.value(K_FORMAT).toString();
    mLastParsed = QDateTime::fromString(object.value(K_LAST_PARSED).toString());
    QJsonArray trackArray = object.value(K_TRACKLIST).toArray();
    foreach (QJsonValue trackValue, trackArray) {
        QJsonObject trackObject = trackValue.toObject();
        TTrackItem *trackItem = new TTrackItem;
        trackItem->musicFile = mFileName;
        trackItem->fromJson(trackObject);
        mTrackItems.append(trackItem);
    }
}

void TMusicItem::sort(SortMode mode)
{
    if(mode==TITLE_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() < b->displayName.toLower();
        });
    else if (mode==TITLE_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() > b->displayName.toLower();
        });
    else if (mode==LENGTH_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() < b->displayName.count();
        });
    else if (mode==LENGTH_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() > b->displayName.count();
        });
    else if (mode==DURATION_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration < b->duration;
        });
    else if (mode==DURATION_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration > b->duration;
        });
    else if (mode==INDEX_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index < b->index;
        });
    else if (mode==INDEX_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index > b->index;
        });
    else if (mode==RANDOM)
    {
        int n = mTrackItems.size()-2;
        for(int i=0;i<mTrackItems.size();i++)
        {
            int index = rand()*n/RAND_MAX;
            TTrackItem *item = mTrackItems.takeAt(index);
            mTrackItems.append(item);
        }
    }
}

QString TMusicItem::displayName()
{
    return mDisplayName;
}

void TMusicItem::setDisplayName(QString displayName)
{
    if(mDisplayName != displayName)
    {
        mDisplayName = displayName;
        mModified = true;
    }
}

QString TMusicItem::fileName()
{
    return mFileName;
}

void TMusicItem::setFileName(QString fileName)
{
    if(mFileName != fileName)
    {
        mFileName = fileName;
        mModified = true;
    }
}

QString TMusicItem::artist()
{
    return mArtist;
}

void TMusicItem::setArtist(QString artist)
{
    if(mArtist != artist)
    {
        mArtist = artist;
        mModified = true;
    }
}

QString TMusicItem::album()
{
    return mAlbum;
}

void TMusicItem::setAlbum(QString album)
{
    if(mAlbum != album)
    {
        mAlbum = album;
        mModified = true;
    }
}

QString TMusicItem::format()
{
    return mFormat;
}

void TMusicItem::setFormat(QString format)
{
    if(mFormat != format)
    {
        mFormat = format;
        mModified = true;
    }
}

int TMusicItem::duration()
{
    return mDuration;
}

void TMusicItem::setDuration(int duration)
{
    if(mDuration != duration)
    {
        mDuration = duration;
        mModified = true;
    }
}

int TMusicItem::fileSize()
{
    return mFileSize;
}

void TMusicItem::setFileSize(int fileSize)
{
    if(mFileSize != fileSize)
    {
        mFileSize = fileSize;
        mModified = true;
    }
}

QDateTime TMusicItem::lastParsed()
{
    return mLastParsed;
}

void TMusicItem::setLastParsed(QDateTime lastParsed)
{
    if(mLastParsed != lastParsed)
    {
        mLastParsed = lastParsed;
        mModified = true;
    }
}

int TMusicItem::size()
{
    return mTrackItems.size();
}

void TMusicItem::clear()
{
    if(mTrackItems.size() <= 0)
        return;

    foreach (TTrackItem *item, mTrackItems) {
        delete item;
    }
    mTrackItems.clear();
    mModified = true;
}
