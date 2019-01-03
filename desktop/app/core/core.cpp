/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "core.h"

#include <QDir>
#include <QMap>
#include <QCoreApplication>

#define tr(x)   qApp->translate("core", x)
void (*LOAD_PLAYLIST_CALLBACK)(int current, int total);

TCore *TCore::mInstance = NULL;

TCore::TCore(bool exportMode) :
    mLibrary(NULL)
  , mSendCmd(NULL)
  , mInitialized(false)
{
    QDir libDir(qApp->applicationDirPath());
    QStringList fileList;
    fileList.append(libDir.absoluteFilePath(FOREPLAYER_LIB_NAME));
    fileList.append(libDir.absoluteFilePath(QString("lib")+FOREPLAYER_LIB_NAME));
    libDir.cd("sdk");
    fileList.append(libDir.absoluteFilePath(FOREPLAYER_LIB_NAME));
    fileList.append(libDir.absoluteFilePath(QString("lib")+FOREPLAYER_LIB_NAME));
    QString libFilePath;
    for(QString filePath : fileList)
    {
        if(QFileInfo(filePath).exists())
        {
            libFilePath = filePath;
            break;
        }
    }
    if(libFilePath.isEmpty())
    {
        mErrorString = tr("Failed to find library %1").arg(FOREPLAYER_LIB_NAME);
        return;
    }
    QFileInfo libFileInfo(libFilePath);
    QString libName = libFileInfo.completeBaseName();
    QString libPath = libFileInfo.absolutePath();
    QString currentPath = QDir::currentPath();
    if(currentPath != libPath)
        QDir::setCurrent(libPath);

    mLibrary = new QLibrary(libFilePath);
    bool loadSuccess = mLibrary->load();
    if(currentPath != libPath)
        QDir::setCurrent(currentPath);

    if(loadSuccess)
    {
        mSendCmd = (FOREPLAYER_SEND_CMD)mLibrary->resolve(FOREPLAYER_SEND_CMD_NAME);
        if(!mSendCmd)
        {
            mErrorString = tr("Failed to resolve proc %1 in library %2.").arg(FOREPLAYER_SEND_CMD_NAME).arg(libName);
            return;
        }
    } else {
        mErrorString = mLibrary->errorString();
        return;
    }
    mSendCmd(CMD_OPEN, &exportMode, &mInitialized, 0, 0);
    if(!mInitialized)
        mErrorString = tr("Failed to initialize sdk %1.").arg(libName);
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

bool TCore::isInitialized()
{
    return mInitialized;
}

QString TCore::getErrorString()
{
    return mErrorString;
}

void TCore::loadPlayList()
{

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
        mSendCmd(CMD_REMOVE_PLAYLIST, &index, 0, 0, 0);
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
    if(mSendCmd)
        mSendCmd(CMD_GET_AUDIO_DATA, &dataType, param1, param2, 0);
}

PlayListItem TCore::getPlayingPlaylistItem()
{
    PlayListItem item = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_CURRENT_PLAYLIST_ITEM, &item, 0, 0, 0);

    return item;
}

MusicItem TCore::getPlayingMusicItem()
{
    MusicItem item = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_CURRENT_MUSIC_ITEM, &item, 0, 0, 0);

    return item;
}

TrackItem TCore::getPlayingTrackItem()
{
    TrackItem item = NULL;
    if(mSendCmd)
        mSendCmd(CMD_GET_CURRENT_TRACK_ITEM, &item, 0, 0, 0);

    return item;
}

int TCore::getCurrentPlayedTime()
{
    int ret = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYED_TIME, &ret, 0, 0, 0);

    return ret;
}

bool TCore::stop()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_STOP, &ret, 0, 0, 0);

    return ret;
}

bool TCore::resume()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_RESUME, &ret, 0, 0, 0);

    return ret;
}

bool TCore::isPaused()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_IS_PAUSED, &ret, 0, 0, 0);

    return ret;
}

bool TCore::isStoped()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_IS_STOPED, &ret, 0, 0, 0);

    return ret;
}

bool TCore::isPlaying()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_IS_PLAYING, &ret, 0, 0, 0);

    return ret;
}

bool TCore::pause()
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_PAUSE, &ret, 0, 0, 0);

    return ret;
}

bool TCore::seek(int microSeconds)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_SEEK_POSITION, &microSeconds, &ret, 0, 0);

    return ret;
}

bool TCore::playTrackItem(TrackItem trackItem)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_PLAY_TRACK, trackItem, &ret, 0, 0);

    return ret;
}

bool TCore::playIndex(int pi, int mi, int ti)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYER_PLAY_INDEX, &pi, &mi, &ti, &ret);

    return ret;
}

void TCore::getPlayingIndex(int *pi, int *mi, int *ti)
{
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYING_INDEX, (void*)pi, (void*)mi, (void*)ti, 0);
}

void TCore::setPlayingIndex(int pi, int mi, int ti)
{
    if(mSendCmd)
        mSendCmd(CMD_SET_PLAYING_INDEX, (void*)&pi, (void*)&mi, (void*)&ti, 0);
}

int TCore::getPlayingIndex(IndexType it)
{
    int pi = -1;
    int mi = -1;
    int ti = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYING_INDEX, &pi, &mi, &ti, 0);

    int ret = -1;
    switch (it) {
    case IT_PL:
        ret = pi;
        break;
    case IT_ML:
        ret = mi;
        break;
    case IT_TL:
        ret = ti;
        break;
    default:
        break;
    }
    return ret;
}

void TCore::setPlayingIndex(IndexType it, int index)
{
    int pi = -1;
    int mi = -1;
    int ti = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLAYING_INDEX, &pi, &mi, &ti, 0);

    switch (it) {
    case IT_PL:
        pi = index;
        break;
    case IT_ML:
        mi = index;
        break;
    case IT_TL:
        ti = index;
        break;
    default:
        break;
    }
    if(mSendCmd)
        mSendCmd(CMD_SET_PLAYING_INDEX, (void*)&pi, (void*)&mi, (void*)&ti, 0);
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

QString TCore::musicItemToString(MusicItem musicItem)
{
    MusicItems musicItems;
    musicItems.append(musicItem);
    return musicItemToString(musicItems);
}

QString TCore::musicItemToString(MusicItems musicItems)
{
    string ret;
    list<MusicItem> musicItemsList = musicItems.toStdList();
    if(mSendCmd)
        mSendCmd(CMD_MUSIC_ITEMS_AS_STRING, &musicItemsList, &ret, 0, 0);

    return QString::fromStdString(ret);
}

MusicItems TCore::stringToMusicItems(QString str)
{
    list<MusicItem> ret;
    string wstr = str.toStdString();
    if(mSendCmd)
        mSendCmd(CMD_STRING_TO_MUSIC_ITEMS, &wstr, &ret, 0, 0);

    return MusicItems::fromStdList(ret);
}

int TCore::getMusicItemIndex(PlayListItem playlistItem, MusicItem musicItem)
{
    int index = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_INDEX, playlistItem, musicItem, &index, 0);

    return index;
}

QList<int> TCore::moveMusicItems(PlayListItem playlistItem, QList<int> indexes, int pos)
{
    list<int> stdList = indexes.toStdList();
    list<int> result;
    if(mSendCmd)
        mSendCmd(CMD_MOVE_MUSIC_ITEMS, playlistItem, &stdList, &pos, &result);

    return QList<int>::fromStdList(result);
}

int TCore::insertMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem)
{
    int ret = -1;
    QList<int> retPos;
    MusicItems musicItems;
    musicItems.append(musicItem);
    if(mSendCmd)
        mSendCmd(CMD_INSERT_MUSIC_ITEMS, playlistItem, &pos, &musicItems, &retPos);

    if(retPos.size() > 0)
        ret = retPos.at(0);

    return ret;
}

QList<int> TCore::insertMusicItems(PlayListItem playlistItem, int pos, MusicItems musicItems)
{
    list<int> ret;
    list<void*> stdList = musicItems.toStdList();
    if(mSendCmd)
        mSendCmd(CMD_INSERT_MUSIC_ITEMS, playlistItem, &pos, &stdList, &ret);

    return QList<int>::fromStdList(ret);
}

QList<int> TCore::removeMusicItems(PlayListItem playlistItem, QList<int> posList)
{
    list<int> ret;
    list<int> stdList = posList.toStdList();
    if(mSendCmd)
        mSendCmd(CMD_REMOVE_MUSIC_ITEMS, playlistItem, &stdList, &ret, 0);

    return QList<int>::fromStdList(ret);
}

bool TCore::removeMusicItem(PlayListItem playlistItem, int pos)
{
    list<int> ret;
    list<int> posList;
    posList.push_back(pos);
    if(mSendCmd)
        mSendCmd(CMD_REMOVE_MUSIC_ITEMS, playlistItem, &posList, &ret, 0);

    return ret.size()==1;
}

bool TCore::updateMusicItem(PlayListItem playlistItem, int pos, MusicItem musicItem)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_PLAYLIST_UPDATE_MUSIC_ITEM, playlistItem, &pos, musicItem, &ret);

    return ret;
}

QList<int> TCore::removeRedundant(PlayListItem playlistItem)
{
    list<int> result;
    if(mSendCmd)
        mSendCmd(CMD_PLAYLIST_REMOVE_REDUNDANT, playlistItem, &result, 0, 0);

    return QList<int>::fromStdList(result);
}

QList<int> TCore::removeErrors(PlayListItem playlistItem)
{
    list<int> result;
    if(mSendCmd)
        mSendCmd(CMD_PLAYLIST_REMOVE_ERRORS, playlistItem, &result, 0, 0);

    return QList<int>::fromStdList(result);
}

void TCore::clear(PlayListItem playlistItem)
{
    if(mSendCmd)
        mSendCmd(CMD_PLAYLIST_ITEM_CLEAR, playlistItem, 0, 0, 0);
}

void TCore::sort(PlayListItem playlistItem, SortMethod sm)
{
    if(mSendCmd)
        mSendCmd(CMD_PLAYLIST_ITEM_SORT, playlistItem, &sm, 0, 0);
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
        mSendCmd(CMD_GET_MUSIC_ITEM_FILE_NAME, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemArtist(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_ARTIST, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemAlbum(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_ALBUM, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemType(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_TYPE, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemYear(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_YEAR, musicItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getMusicItemAdditionalInfo(MusicItem musicItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_MUSIC_ITEM_ADDTIONAL_INFO, musicItem, &ret, 0, 0);

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

TrackItems TCore::getTrackItems(MusicItem musicItem)
{
    list<TrackItem> result;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEMS, musicItem, &result, 0, 0);

    return QList<TrackItem>::fromStdList(result);
}

QString TCore::trackItemToString(TrackItem trackItem)
{
    TrackItems trackItems;
    trackItems.append(trackItem);
    return trackItemToString(trackItems);
}

QString TCore::trackItemToString(TrackItems trackItems)
{
    wstring ret;
    list<TrackItem> trackItemsList = trackItems.toStdList();
    if(mSendCmd)
        mSendCmd(CMD_TRACK_ITEMS_AS_STRING, &trackItemsList, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

int TCore::getTrackItemIndex(MusicItem musicItem, TrackItem trackItem)
{
    int ret = -1;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_INDEX, musicItem, trackItem, &ret, 0);

    return ret;
}

QString TCore::getTrackItemName(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_NAME, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

bool TCore::setTrackItemName(MusicItem musicItem, TrackItem trackItem, QString newName)
{
    bool ret = false;
    wstring newNameW = newName.toStdWString();
    if(mSendCmd)
        mSendCmd(CMD_SET_TRACK_ITEM_NAME, musicItem, trackItem, &newNameW, &ret);

    return ret;
}

bool TCore::setTrackItemDuration(MusicItem musicItem, TrackItem trackItem, int duration)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_SET_TRACK_ITEM_DURATION, musicItem, trackItem, &duration, &ret);

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
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_TYPE, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getTrackItemArtist(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_ARTIST, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getTrackItemAlbum(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_ALBUM, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getTrackItemYear(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_YEAR, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getTrackItemAdditionalInfo(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_ADDITIONAL_INFO, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getTrackItemIndexName(TrackItem trackItem)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_INDEX_NAME, trackItem, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

int TCore::getTrackItemSampleRate(TrackItem trackItem)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_GET_TRACK_ITEM_SAMPLE_RATE, trackItem, &ret, 0, 0);

    return ret;
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

PluginHandles TCore::getPluginHandles()
{
    list<PluginHandle> pluginHandles;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_LIST, &pluginHandles, 0, 0, 0);

    return PluginHandles::fromStdList(pluginHandles);
}
QString TCore::getPluginName(PluginHandle pluginHandle)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_NAME, pluginHandle, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getPluginManufacture(PluginHandle pluginHandle)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_MANUFACTURE, pluginHandle, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getPluginContact(PluginHandle pluginHandle)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_CONTACT, pluginHandle, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getPluginCreateDate(PluginHandle pluginHandle)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_CREATE_DATE, pluginHandle, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QString TCore::getPluginDescription(PluginHandle pluginHandle)
{
    wstring ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_DESCRIPTION, pluginHandle, &ret, 0, 0);

    return QString::fromStdWString(ret);
}

QList<QPair<QString, QString>> TCore::getPluginSuffixDescription(PluginHandle pluginHandle)
{
    map<wstring, wstring> ret;
    if(mSendCmd)
        mSendCmd(CMD_GET_PLUGIN_SUFFIXDESCRIPTION, pluginHandle, &ret, 0, 0);

    map<wstring, wstring>::iterator it;
    QList<QPair<QString, QString>> _ret;
    for(it=ret.begin();it!=ret.end();it++)
        _ret.append(qMakePair(QString::fromStdWString(it->first), QString::fromStdWString(it->second)));

    return _ret;
}

bool TCore::loadTrackItem(TrackItem trackItem)
{
    bool ret = false;
    if(mSendCmd)
        mSendCmd(CMD_EXPORT_LOAD_TRACK, trackItem, &ret, 0, 0);

    return ret;
}

void TCore::getNextFrame(char *buffer, int size)
{
    if(mSendCmd)
        mSendCmd(CMD_EXPORT_NEXT_FRAME, buffer, &size, 0, 0);
}

int TCore::getFrameSampleCount(int sampleRate, int fps)
{
    int ret = 0;
    if(mSendCmd)
        mSendCmd(CMD_EXPORT_FRAME_SAMPLE_COUNT, &sampleRate, &fps, &ret, 0);

    return ret;
}
