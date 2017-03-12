#include "playlistitem.h"

#include "playlistdef.h"

TPlaylistItem::TPlaylistItem(QString fileName) :
    mFileName(fileName)
  , mModified(false)
{
}

TPlaylistItem::~TPlaylistItem()
{
    clear();
}

QJsonObject TPlaylistItem::toJson()
{
    QJsonObject object;
    //Set the data of the playlist.
    object[K_NAME] = mDisplayName;
    object[K_FILE] = mFileName;
    object[K_VERSION] = mVersion;

    QJsonArray musicsArray;
    foreach (TMusicItem *musicItem, mMusicItems) {
        musicsArray.append(musicItem->toJson());
    }
    object[K_MUSICLIST] = musicsArray;
    return object;
}

void TPlaylistItem::fromJson(QJsonObject object)
{
    mDisplayName = object.value(K_NAME).toString(K_DEFAULT);
    mVersion = object.value(K_VERSION).toInt(PL_VERSION);
    QJsonArray musicArray = object.value(K_MUSICLIST).toArray();
    foreach (QJsonValue musicValue, musicArray) {
        QJsonObject musicObject = musicValue.toObject();
        TMusicItem *musicItem = new TMusicItem;
        musicItem->fromJson(musicObject);
        mMusicItems.append(musicItem);
    }
}

TMusicItem *TPlaylistItem::takeAt(int index)
{
    if(index<0 || index>=mMusicItems.size())
        return NULL;

    mModified = true;
    return mMusicItems.takeAt(index);
}

void TPlaylistItem::insert(int pos, TMusicItem *item)
{
    if(!item)
        return;

    mMusicItems.insert(pos, item);
    mModified = true;
}

void TPlaylistItem::update(int index, TMusicItem *item)
{
    if(index<0 || index>=mMusicItems.size())
        return;

    TMusicItem *oldItem = mMusicItems[index];
    mMusicItems[index] = item;
    delete oldItem;
    mModified = true;
}

bool TPlaylistItem::remove(int index)
{
    if(index<0 || index>=mMusicItems.size())
        return false;

    delete mMusicItems[index];
    mMusicItems.removeAt(index);
    mModified = true;
    return true;
}

QList<int> TPlaylistItem::removeRedundant()
{
    QSet<int> removedSet;
    int itemSize = mMusicItems.size();
    for(int i=0;i<itemSize;i++)
    {
        TMusicItem *item1 = mMusicItems[i];
        for(int j=i+1;j<itemSize;j++)
        {
            TMusicItem *item2 = mMusicItems[j];
            if(item1->fileName().toLower() == item2->fileName().toLower())
            {
                removedSet.insert(j);
            }
        }
    }
    QList<int> removedList = removedSet.toList();
    int removedSize = removedList.size();
    if(removedSize > 0)
    {
        std::sort(removedList.begin(), removedList.end(), [=](int a, int b) {
            return a > b;
        });

        foreach (int i, removedList) {
            delete mMusicItems[i];
            mMusicItems.removeAt(i);
        }
        mModified = true;
    }
    return removedList;
}

QList<int> TPlaylistItem::removeErrors()
{
    QList<int> removed;

    for(int i=0;i<mMusicItems.size();i++)
    {
        TMusicItem *musicItem = mMusicItems[i];
        if(!QFile::exists(musicItem->fileName()) || (musicItem->duration()<=0 && musicItem->trackItems()->size()<=0))
            removed.append(i);
    }

    int removedSize = removed.size();
    if(removedSize > 0)
    {
        std::sort(removed.begin(), removed.end());

        for(int i=removed.size()-1;i>=0;i--)
        {
            int index = removed[i];
            delete mMusicItems[index];
            mMusicItems.removeAt(index);
        }
        mModified = true;
    }
    return removed;
}

void TPlaylistItem::setDisplayName(QString newName)
{
    if(mDisplayName != newName)
    {
        mDisplayName = newName;
        mModified = true;
    }
}

void TPlaylistItem::sort(SortMethod mode)
{
    if(mode==SM_TITLE_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().toLower() < b->displayName().toLower();
        });
    else if (mode==SM_TITLE_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().toLower() > b->displayName().toLower();
        });
    else if (mode==SM_LENGTH_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().count() < b->displayName().count();
        });
    else if (mode==SM_LENGTH_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->displayName().count() > b->displayName().count();
        });
    else if (mode==SM_DURATION_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration() < b->duration();
        });
    else if (mode==SM_DURATION_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->duration() > b->duration();
        });
    else if (mode==SM_FILE_ASC)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName() < b->fileName();
        });
    else if (mode==SM_FILE_DES)
        std::sort(mMusicItems.begin(), mMusicItems.end(), [=](TMusicItem *a, TMusicItem *b){
            return a->fileName() > b->fileName();
        });
    else if (mode==SM_RANDOM)
    {
        int n = mMusicItems.size()-2;
        for(int i=0;i<mMusicItems.size();i++)
        {
            int index = rand()*n/RAND_MAX;
            TMusicItem *item = mMusicItems.takeAt(index);
            mMusicItems.append(item);
        }
    } else if (mode==SM_REVERSE) {
        TMusicItems musicItems;
        for(int i=0;i<mMusicItems.size();i++)
        {
            musicItems.prepend(mMusicItems[i]);
        }
        mMusicItems = musicItems;
    }
}

int TPlaylistItem::indexOf(TMusicItem *item)
{
    return mMusicItems.indexOf(item);
}

TMusicItem *TPlaylistItem::musicItem(int index)
{
    if(index<0 || index>=mMusicItems.size())
        return NULL;

    return mMusicItems[index];
}

int TPlaylistItem::musicItemIndex(TMusicItem *musicItem)
{
    return mMusicItems.indexOf(musicItem);
}

void TPlaylistItem::setFileName(QString fileName)
{
    if(mFileName != fileName)
    {
        mFileName = fileName;
        mModified = true;
    }
}

void TPlaylistItem::save()
{
    bool modified = mModified;
    foreach (TMusicItem *item, mMusicItems) {
        modified |= item->isModified();
    }
    if(modified)
    {
        QFile file(mFileName);

        if(!file.open(QIODevice::WriteOnly))
            return;

        QJsonDocument playlistDocument;
        //Set the playlist object to document object.
        playlistDocument.setObject(toJson());
        //Write document data to the file.
        file.write(qCompress(playlistDocument.toJson(QJsonDocument::Indented)));
        //Close the file.
        file.close();

        foreach (TMusicItem *item, mMusicItems) {
            item->setModified(false);
        }
        mModified = false;
    }
}

void TPlaylistItem::clear()
{
    if(mMusicItems.size() <= 0)
        return;

    foreach (TMusicItem *item, mMusicItems) {
        delete item;
    }
    mMusicItems.clear();
    mModified = true;
}

int TPlaylistItem::size()
{
    return mMusicItems.size();
}
