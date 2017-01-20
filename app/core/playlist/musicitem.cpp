#include "musicitem.h"

#include "playlistdef.h"

TMusicItem::TMusicItem() :
    plugin(NULL)
    , mDuration(0)
    , mFileSize(0)
    , mModified(false)
{
    mLastParsed = QDateTime::currentDateTime();
}

TMusicItem::~TMusicItem()
{
    plugin = NULL;
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
        trackItem->musicFilePath = &mFileName;
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
