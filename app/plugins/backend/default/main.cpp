
#include "../backendinterface.h"
#include "bass.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <QDataStream>

const char *szName = "Bass";
const char *szManufacture = "bilge theall";
const char *szContact = "wazcd_1608@qq.com";
const char *szDescription = "Bass plugin.http://www.un4seen.com/";
const char *szCreateDate = "2017-02-19";
const int g_sampleRate = 44100;
const int g_bufSize = 20000;

HSTREAM g_handle;

struct SuffixDesc
{
    char *suffix;
    char *desc;
};

const SuffixDesc szTypeDesc[] =
{
    {(char*)"wav", (char*)"Wave"},
    {(char*)"mp3", (char*)"Mpeg 1 layer 3"},

    {NULL, NULL}
};

char g_SuffixList[256];
QString szError;

std::string toStdString(const char *str)
{
    return QString::fromLocal8Bit(str).toStdString();
}

// Initialize plugin
EXPORT bool initialize()
{
    int i = 0;
    g_SuffixList[0] = '\0';
    while (szTypeDesc[i].suffix) {
        strcat(g_SuffixList, szTypeDesc[i].suffix);
        strcat(g_SuffixList, " ");
        i++;
    }

    if (!BASS_Init(0, g_sampleRate, 0, 0, NULL))
        return false;

    return true;
}

// Verify this plugin can parse specify suffix of file
EXPORT const char *matchSuffixes()
{
    return g_SuffixList;
}

// Parse file to get details information
EXPORT bool parse(const wchar_t *file, TMusicInfo* musicInfo)
{
    QString qFile = QString::fromWCharArray(file);
    wstring fileW = qFile.toStdWString();

    HSAMPLE handle = BASS_StreamCreateFile(FALSE, fileW.c_str(), 0, 0, BASS_STREAM_DECODE);
    if(!handle)
        return false;

    TAG_ID3 *id3=(TAG_ID3*)BASS_ChannelGetTags(handle, BASS_TAG_ID3); // get the ID3 tags
    TTrackInfo *trackInfo = new TTrackInfo;

    if(id3)
    {
        QString title = toStdString(id3->title).c_str();
        trackInfo->trackName = title.toStdString();
        trackInfo->additionalInfo = QString::asprintf(
                        "artist: %s\n"
                        "album: %s\n"
                        "year: %s\n"
                        "comment: %s",
                        toStdString(id3->artist).c_str(),
                        toStdString(id3->album).c_str(),
                        toStdString(id3->year).c_str(),
                        toStdString(id3->comment).c_str()
                        ).toStdString();

    }

    if(trackInfo->trackName=="")
        trackInfo->trackName = QFileInfo(qFile).baseName().toStdString();

    DWORD pos = BASS_ChannelGetLength(handle, BASS_POS_BYTE);
    if (pos && pos!=(DWORD)-1) {
        musicInfo->fileSize = pos;
        DWORD seconds = (DWORD)BASS_ChannelBytes2Seconds(handle, pos);
        trackInfo->duration = seconds * 1000;
    }

    musicInfo->musicName = trackInfo->trackName;
    musicInfo->duration = trackInfo->duration;
    musicInfo->additionalInfo = trackInfo->additionalInfo;

    musicInfo->trackList.push_back(trackInfo);

    BASS_StreamFree(handle);

    //Mission complete.
    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    QFileInfo fi(trackInfo->musicFileName.c_str());

    if(!fi.exists())
        return false;

    wstring fileNameW = fi.absoluteFilePath().toStdWString();

    if(g_handle)
        BASS_StreamFree(g_handle);

    g_handle = BASS_StreamCreateFile(FALSE, fileNameW.c_str(), 0, 0, BASS_STREAM_DECODE);
    return g_handle != 0;
}

// Seek time
EXPORT bool seek(int microSeconds)
{
    (void)microSeconds;

    return false;
}

// Close track
EXPORT void closeTrack()
{
    if(g_handle)
    {
        BASS_StreamFree(g_handle);
        g_handle = 0;
    }
}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{
    if(g_handle && BASS_ChannelIsActive(g_handle))
        BASS_ChannelGetData(g_handle, samples, size*sizeof(short)*2);
}

// Retrieve plugin information
EXPORT void pluginInformation(TPluginInfo *pluginInfo)
{
    if(!pluginInfo)
        return;
    
    pluginInfo->name = szName;
    pluginInfo->manufacture = szManufacture;
    pluginInfo->contact = szContact;
    pluginInfo->description = szDescription;
    pluginInfo->createDate = szCreateDate;
}

// Use to free plugin
EXPORT void destroy()
{
    closeTrack();
}
