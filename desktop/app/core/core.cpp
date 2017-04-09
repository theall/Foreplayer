#include "core.h"

#include <QDir>
#include <QCoreApplication>

#define tr(x)   qApp->translate("core", x)

TCore *TCore::mInstance = NULL;

TCore::TCore() :
    mLibrary(NULL)
  , mSendCmd(NULL)
{
    QDir libDir(qApp->applicationDirPath());
    QString libFilePath = libDir.absoluteFilePath(FOREPLAYER_LIB_NAME);
    mLibrary = new QLibrary(libFilePath);
    if(mLibrary->load())
    {
        mSendCmd = (FOREPLAYER_SEND_CMD)mLibrary->resolve(FOREPLAYER_SEND_CMD_NAME);
        if(!mSendCmd)
            throw tr("Failed to resolve proc %1 in library %2").arg(FOREPLAYER_SEND_CMD_NAME).arg(FOREPLAYER_LIB_NAME);
    } else {
        throw tr("Failed to load library %1").arg(FOREPLAYER_LIB_NAME);
    }
    mSendCmd(CMD_OPEN, 0, 0, 0, 0);
}

TCore::~TCore()
{
    if(mSendCmd)
    {
        mSendCmd(CMD_CLOSE, 0, 0, 0, 0);
        mSendCmd = NULL;
    }
    if(mLibrary)
    {
        delete mLibrary;
        mLibrary = NULL;
    }
}

TCore *TCore::instance()
{
    if(!mInstance)
        mInstance = new TCore;

    return mInstance;
}

void TCore::deleteInstance()
{
    if(mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

int TCore::playlistCount()
{
    vector<wstring> ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYLISTS_NAME_LIST, &ret, 0, 0, 0);

    return ret.size();
}

QStringList TCore::playlists()
{
    vector<wstring> ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYLISTS_NAME_LIST, &ret, 0, 0, 0);

    QStringList qRet;
    for(wstring item : ret)
    {
        qRet.append(QString::fromStdWString(item));
    }

    return qRet;
}

int TCore::insertPlaylist(QString name, int pos)
{
    int ret = -1;
    wstring wname = name.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_INSERT_PLAYLIST, &wname, &pos, &ret, 0);

    return ret;
}

void TCore::removePlaylist(int index)
{
    if(mSendCmd)
        mSendCmd(CMD_INSERT_PLAYLIST, &index, 0, 0, 0);
}

QList<int> TCore::movePlaylists(QList<int> indexes, int pos)
{
    list<int> stdList = indexes.toStdList();
    list<int> result;
    if(mSendCmd)
        mSendCmd(CMD_MOVE_PLAYLISTS, &stdList, &pos, &result, 0);

    return QList<int>::fromStdList(result);
}

void TCore::renamePlaylist(int index, QString name)
{
    wstring wName = name.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_RENAME_PLAYLIST, &index, &wName, 0, 0);
}

void TCore::sort(SortMethod sm)
{
    if(mSendCmd)
        mSendCmd(CMD_SORT_PLAYLISTS, &sm, 0, 0, 0);
}

PlayListItem TCore::getPlaylistItem(int index)
{
    PlayListItem item = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYLIST_ITEM, &index, &item, 0, 0);

    return item;
}

int TCore::getPlaylistItemIndex(PlayListItem item)
{
    int index = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYLIST_ITEM_INDEX, item, &index, 0, 0);

    return index;
}

QString TCore::getPlaylistName(int index)
{
    wstring name;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYLIST_NAME, &index, &name, 0, 0);

    return QString::fromStdWString(name);
}

void TCore::setPlaylistName(int index, QString newName)
{
    wstring wName = newName.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_SET_PLAYLIST_NAME, &index, &wName, 0, 0);
}

void TCore::setAudioParameter(AudioParameter type, float value, int param)
{
    if(mSendCmd)
        mSendCmd(CMD_SET_AUDIO_PARAMETER, &type, &value, &param, 0);
}

void TCore::getAudioData(AudioDataType dataType, void *param1, void *param2)
{

}

PlayListItem TCore::getPlayingPlaylistItem()
{

}

MusicItem TCore::getPlayingMusicItem()
{

}

TrackItem TCore::getPlayingTrackItem()
{
    return NULL;
}

int TCore::getCurrentPlayedTime()
{

}

bool TCore::stop()
{

}

bool TCore::resume()
{

}

bool TCore::isPaused()
{

}

bool TCore::isStoped()
{

}

bool TCore::isPlaying()
{

}

bool TCore::pause()
{

}

bool TCore::playTrackItem(TrackItem trackItem)
{
    return false;
}

bool TCore::playIndex(int pi, int mi, int ti)
{

}

void TCore::getPlayingIndex(int *pi, int *mi, int *ti)
{

}

int TCore::getPlayingIndex(IndexType it)
{

}

int TCore::getMusicItemCount(PlayListItem playlistItem)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_COUNT, playlistItem, &ret, 0, 0);

    return ret;
}

MusicItem TCore::getMusicItem(PlayListItem playlistItem, int row)
{
    MusicItem ret = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM, playlistItem, &row, &ret, 0);

    return ret;
}

int TCore::getMusicItemIndex(PlayListItem playlistItem, MusicItem musicItem)
{
    int index = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_INDEX, playlistItem, musicItem, &index, 0);

    return index;
}

QList<int> TCore::moveMusicItems(QList<int> indexes, int pos)
{
    list<int> stdList = indexes.toStdList();
    list<int> result;
    if(mSendCmd)
        mSendCmd(CMD_MOVE_MUSIC_ITEMS, &stdList, &pos, &result, 0);

    return QList<int>::fromStdList(result);
}

int TCore::insertMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem)
{
    int ret = -1;
    if(mSendCmd)
        mSendCmd(CMD_INSERT_MUSIC_ITEM, playlistItem, &pos, musicItem, &ret);

    return ret;
}

bool TCore::removeMusicItem(PlayListItem playlistItem, int pos)
{
    bool ret = true;
    if(mSendCmd)
        mSendCmd(CMD_REMOVE_MUSIC_ITEM, playlistItem, &pos, &ret, 0);

    return ret;
}

bool TCore::updateMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem)
{

}

QList<int> TCore::removeRedundant(PlayListItem playlistItem)
{

}

QList<int> TCore::removeErrors(PlayListItem playlistItem)
{

}

void TCore::clear(PlayListItem playlistItem)
{

}

void TCore::sort(PlayListItem playlistItem, SortMethod sm)
{

}

QString TCore::getMusicItemDisplayName(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemFileName(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemArtist(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemAlbum(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemType(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemYear(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemAdditionalInfo(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

int TCore::getMusicItemDuration(MusicItem musicItem)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_DURATION, musicItem, &ret, 0, 0);

    return ret;
}

bool TCore::setMusicItemName(MusicItem musicItem, QString newName)
{
    bool ret = false;
    wstring newNameW = newName.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_SET_MUSIC_ITEM_DISPLAY_NAME, musicItem, &newNameW, &ret, 0);

    return ret;
}

int TCore::getTrackItemCount(MusicItem musicItem)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_COUNT, musicItem, &ret, 0, 0);

    return ret;
}

TrackItem TCore::getTrackItem(MusicItem musicItem, int index)
{
    TrackItem ret = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM, musicItem, &index, &ret, 0);

    return ret;
}

TTrackItems TCore::getTrackItems(MusicItem musicItem)
{

}

int TCore::getTrackItemIndex(MusicItem musicItem, TrackItem trackItem)
{
    int ret = -1;
    if(!musicItem || !trackItem)
        return ret;
}

QString TCore::getTrackItemName(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_NAME, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

bool TCore::setTrackItemName(TrackItem trackItem, QString newName)
{
    bool ret = false;
    wstring newNameW = newName.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_SET_TRACK_ITEM_NAME, trackItem, &newNameW, &ret, 0);

    return ret;
}

bool TCore::setTrackItemDuration(TrackItem trackItem, int duration)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_SET_TRACK_ITEM_DURATION, trackItem, &duration, &ret, 0);

    return ret;
}

int TCore::getTrackItemDuration(TrackItem trackItem)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_DURATION, trackItem, &ret, 0, 0);

    return ret;
}

QString TCore::getTrackItemType(TrackItem trackItem)
{

}

QString TCore::getTrackItemArtist(TrackItem trackItem)
{

}

QString TCore::getTrackItemAlbum(TrackItem trackItem)
{

}

QString TCore::getTrackItemYear(TrackItem trackItem)
{

}

QString TCore::getTrackItemAdditionalInfo(TrackItem trackItem)
{

}

QString TCore::getTrackItemIndexName(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_INDEX_NAME, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

bool TCore::isTrackItemEnabled(TrackItem trackItem)
{
    bool ret = true;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_ENABLED, trackItem, &ret, 0, 0);

    return ret;
}

MusicItem TCore::parse(QString filePath)
{
    MusicItem ret = NULL;
    wstring filePathW = filePath.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_PARSE_FILE, &filePathW, &ret, 0, 0);

    return ret;
}
