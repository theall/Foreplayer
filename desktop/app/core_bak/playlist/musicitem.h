#ifndef TMUSICITEM_H
#define TMUSICITEM_H

#include "trackitem.h"

enum SortMethod
{
    SM_TITLE_ASC,
    SM_TITLE_DES,
    SM_LENGTH_ASC,
    SM_LENGTH_DES,
    SM_DURATION_ASC,
    SM_DURATION_DES,
    SM_INDEX_ASC,
    SM_INDEX_DES,
    SM_FILE_ASC,
    SM_FILE_DES,
    SM_ARTIST_ASC,
    SM_ARTIST_DES,
    SM_ALBUM_ASC,
    SM_ALBUM_DES,
    SM_GAME_ASC,
    SM_GAME_DES,
    SM_SYSTEM_ASC,
    SM_SYSTEM_DES,
    SM_YEAR_ASC,
    SM_YEAR_DES,
    SM_RANDOM,
    SM_REVERSE
};

class TMusicItem
{
public:
    TMusicItem();
    ~TMusicItem();

    QJsonObject toJson();

    void fromJson(QJsonObject object);

    void fromTrackItem(TTrackItem *trackItem);

    void sort(SortMethod mode);

    std::wstring displayName();
    void setDisplayName(std::wstring displayName);

    std::wstring originalName();
    void setOriginalName(std::wstring name);

    std::wstring fileName();
    void setFileName(std::wstring fileName);

    std::wstring system();
    void setSystem(std::wstring system);

    std::wstring artist();
    void setArtist(std::wstring artist);

    std::wstring year();
    void setYear(std::wstring year);

    std::wstring additionalInfo();
    void setAdditionalInfo(std::wstring info);

    int duration();
    void setDuration(int duration);

    int fileSize();
    void setFileSize(int fileSize);

    std::wstring game();
    void setGame(std::wstring game);

    std::wstring album();
    void setAlbum(std::wstring album);

    QDateTime lastParsed();
    void setLastParsed(QDateTime lastParsed);

    TTrackItems *trackItems();
    TTrackItem *trackItem(int index);
    void addTrackItem(TTrackItem *trackItem, int pos=-1);

    bool isModified();
    void setModified(bool modified=true);

    int size();
    void clear();

    static std::string mimeType() { return "forplayer/musicitem"; }

private:
    std::wstring mDisplayName;
    std::wstring mOriginalName;
    std::wstring mFileName;
    std::wstring mGame;
    std::wstring mSystem;
    std::wstring mArtist;
    std::wstring mYear;
    std::wstring mAdditionalInfo;
    int mDuration;
    int mFileSize;
    bool mModified;
    QDateTime mLastParsed;
    TTrackItems mTrackItems;
};

typedef QList<TMusicItem*> TMusicItems;

#endif // TMUSICITEM_H
