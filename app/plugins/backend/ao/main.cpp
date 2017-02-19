
#include "../backendinterface.h"
#include "../../../../thirdparty/aosdk/aosdk.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QLibrary>
#include <QFileInfo>
#include <QDataStream>

const char *szName = "Audio overload";
const char *szManufacture = "bilge theall";
const char *szContact = "wazcd_1608@qq.com";
const char *szDescription = "Audio overload plugin.";
const char *szCreateDate = "2017-02-18";

struct SuffixDesc
{
    char *suffix;
    char *desc;
};

const SuffixDesc szTypeDesc[] =
{
    {(char*)"qsf", (char*)"Capcom QSound"},
    {(char*)"ssf", (char*)"Sega Saturn"},
    {(char*)"psf", (char*)"Sony PlayStation"},
    {(char*)"spu", (char*)"Sony PlayStation"},
    {(char*)"psf2", (char*)"Sony PlayStation 2"},
    {(char*)"dsf", (char*)"Sega Dreamcast"},
    {(char*)"dsf", (char*)"Sega Dreamcast"},
    {(char*)"dsf", (char*)"Sega Dreamcast"},

    {NULL, NULL}
};

char g_SuffixList[256];
QString szError;

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
    AO_MUSIC_INFO aoInfo;
    int result = ao_music_info(file, &aoInfo);
    if(!result)
        return false;

    TTrackInfo *trackInfo = new TTrackInfo;
    trackInfo->trackName = aoInfo.name;
    if(trackInfo->trackName=="")
        trackInfo->trackName = QFileInfo(QString::fromWCharArray(file)).baseName().toStdString();

    trackInfo->duration = aoInfo.length;

    trackInfo->additionalInfo = QString::asprintf(
                    "game: %s\n"
                    "author: %s\n"
                    "copyright: %s",
                    aoInfo.game,
                    aoInfo.artist,
                    aoInfo.copyright
                    ).toStdString();

    musicInfo->musicName = trackInfo->trackName;
    musicInfo->duration = trackInfo->duration;
    musicInfo->additionalInfo = trackInfo->additionalInfo;
    musicInfo->trackList.push_back(trackInfo);

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

    return ao_load_file(fileNameW.c_str());
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
    ao_exit();
    //m1snd_run(M1_CMD_STOP, 0);
}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{
    // size is the size of 1 channel samples
    ao_do_frame((unsigned long)size, (unsigned char*)samples);
}

// Optional, for return customized sample size
EXPORT int sampleSize(int sampleRate, int fps)
{
    // 1 channel
    (void)fps;
    return sampleRate/60;
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
    ao_exit();
}
