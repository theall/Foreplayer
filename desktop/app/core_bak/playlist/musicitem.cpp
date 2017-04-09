#include "musicitem.h"

#include "playlistdef.h"

TMusicItem::TMusicItem() :
    mDuration(0)
    , mFileSize(0)
    , mModified(false)
{
    mLastParsed = QDateTime::currentDateTime();
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
    object[K_ADDITIONAL] = mAdditionalInfo;
    object[K_DURATION] = mDuration;
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
    mAdditionalInfo = object.value(K_ADDITIONAL).toString();
    mLastParsed = QDateTime::fromString(object.value(K_LAST_PARSED).toString());
    QJsonArray trackArray = object.value(K_TRACKLIST).toArray();
    foreach (QJsonValue trackValue, trackArray) {
        QJsonObject trackObject = trackValue.toObject();
        TTrackItem *trackItem = new TTrackItem;
        trackItem->fromJson(trackObject);\
        mTrackItems.append(trackItem);
    }
}

void TMusicItem::fromTrackItem(TTrackItem *trackItem)
{
    if(!trackItem)
        return;

    mDisplayName = trackItem->displayName;
    mOriginalName = trackItem->originalName;
    mFileName = trackItem->fileName;
    mSystem = trackItem->system;
    mArtist = trackItem->artist;
    mYear = trackItem->year;
    mAdditionalInfo = trackItem->additionalInfo;
    mDuration = trackItem->duration;
    mModified = true;
}

void TMusicItem::sort(SortMethod mode)
{
    if(mode==SM_TITLE_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() < b->displayName.toLower();
        });
    else if (mode==SM_TITLE_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.toLower() > b->displayName.toLower();
        });
    else if (mode==SM_LENGTH_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() < b->displayName.count();
        });
    else if (mode==SM_LENGTH_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->displayName.count() > b->displayName.count();
        });
    else if (mode==SM_DURATION_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration < b->duration;
        });
    else if (mode==SM_DURATION_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->duration > b->duration;
        });
    else if (mode==SM_INDEX_ASC)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index < b->index;
        });
    else if (mode==SM_INDEX_DES)
        std::sort(mTrackItems.begin(), mTrackItems.end(), [=](TTrackItem *a, TTrackItem *b){
            return a->index > b->index;
        });
    else if (mode==SM_RANDOM)
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

QString TMusicItem::originalName()
{
    return mOriginalName;
}

void TMusicItem::setOriginalName(QString name)
{
    mOriginalName = name;
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

QString TMusicItem::system()
{
     return mSystem;
}

void TMusicItem::setSystem(QString system)
{
     if(mSystem == system)
          return;

     mSystem = system;
     mModified = true;
}

QString TMusicItem::artist()
{
     return mArtist;
}

void TMusicItem::setArtist(QString artist)
{
     if(mArtist == artist)
          return;

     mArtist = artist;
     mModified = true;
}

QString TMusicItem::year()
{
     return mYear;
}

void TMusicItem::setYear(QString year)
{
     if(mYear == year)
          return;

     mYear = year;
     mModified = true;
}

QString TMusicItem::additionalInfo()
{
    return mAdditionalInfo;
}

void TMusicItem::setAdditionalInfo(QString info)
{
    if(mAdditionalInfo==info)
        return;

    mAdditionalInfo = info;
    mModified = true;
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

QString TMusicItem::game()
{
    return mGame;
}

void TMusicItem::setGame(QString game)
{
    if(mGame != game)
    {
        mGame = game;
        mModified = true;
    }
}

QString TMusicItem::album()
{
    return mGame;
}

void TMusicItem::setAlbum(QString album)
{
    if(mGame != album)
    {
        mGame = album;
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

TTrackItems *TMusicItem::trackItems()
{
    return &mTrackItems;
}

TTrackItem *TMusicItem::trackItem(int index)
{
    if(index<0 || index>=mTrackItems.size())
        return NULL;

    return mTrackItems.at(index);
}

void TMusicItem::addTrackItem(TTrackItem *trackItem, int pos)
{
    if(pos == -1)
        mTrackItems.append(trackItem);
    else
        mTrackItems.insert(pos, trackItem);
}

bool TMusicItem::isModified()
{
    return mModified;
}

void TMusicItem::setModified(bool modified)
{
    mModified = modified;
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
