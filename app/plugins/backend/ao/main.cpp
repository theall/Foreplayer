
#include "../../../../thirdparty/aosdk/aosdk.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QLibrary>
#include <QFileInfo>
#include <QDataStream>

#include "zipparse.h"

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

const SuffixDesc g_typeDesc[] =
{
    {(char*)"qsf", (char*)"Capcom QSound"},
    {(char*)"ssf", (char*)"Sega Saturn"},
    {(char*)"psf", (char*)"Sony PlayStation"},
    {(char*)"spu", (char*)"Sony PlayStation"},
    {(char*)"psf2", (char*)"Sony PlayStation 2"},
    {(char*)"dsf", (char*)"Sega Dreamcast"},
    {(char*)"minidsf", (char*)"Sega Dreamcast Track"},
    {(char*)"minipsf", (char*)"Game Boy Advance Track"},
    {(char*)"zip", (char*)"Track list package"},

    {NULL, NULL}
};

char g_SuffixList[256];
QString szError;
QString g_FileContext;

void libCallback(const char *fileName, char **buffer, long *length)
{
    *buffer = NULL;
    *length = 0;

    QFileInfo fi(g_FileContext);
    if(fi.isDir())
    {
        // Lib file is in the same directory of track file
        QDir dir(g_FileContext);
        QString targetFile = dir.absoluteFilePath(fileName);
        QFileInfo targetFileInfo(targetFile);
        int fileSize = targetFileInfo.size();
        if(fileSize > 0)
        {
            char *buf = (char*)malloc(fileSize);
            QFile file(targetFile);
            if(file.open(QIODevice::ReadOnly))
            {
                int sizeRead = file.read(buf, fileSize);
                *buffer = buf;
                *length = sizeRead;
            }
        }
    } else {
        // Lib file is in a zip archive package
        TZipParse zipParser(g_FileContext);
        TTrackInfo trackInfo;
        trackInfo.musicFileName = g_FileContext.toStdString();
        trackInfo.indexName = fileName;
        QByteArray data = zipParser.trackData(&trackInfo);
        int fileSize = data.size();
        char *buf = (char*)malloc(fileSize);
        memcpy(buf, data.constData(), fileSize);
        *buffer = buf;
        *length = fileSize;
    }
}

// Initialize plugin
EXPORT bool initialize()
{
    int i = 0;
    g_SuffixList[0] = '\0';
    while (g_typeDesc[i].suffix) {
        strcat(g_SuffixList, g_typeDesc[i].suffix);
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
    QString fileName = QString::fromWCharArray(file);
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix().toLower();
    int i = 0;
    while(true)
    {
        if(g_typeDesc[i].suffix==suffix || g_typeDesc[i].suffix==NULL)
            break;
        i++;
    }
    if(g_typeDesc[i].suffix == NULL)
        return false;

    bool result = false;
    if(suffix=="zip")
    {
        TZipParse zipParser(fileName);
        result = zipParser.parse(musicInfo);
    } else {
        TFileParse fileParser(fileName);
        result = fileParser.parse(musicInfo);
    }
    return result;

    //Mission complete.
    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    QFileInfo fi(trackInfo->musicFileName.c_str());

    if(!fi.exists() || !fi.isFile())
    {
        g_FileContext.clear();
        return false;
    }

    if(fi.suffix().toLower() == "zip")
    {
        g_FileContext = fi.absoluteFilePath();
        TZipParse zipParser(g_FileContext);
        QByteArray data = zipParser.trackData(trackInfo);
        int fileSize = data.size();
        char *buf = (char*)malloc(fileSize);
        memcpy(buf, data.constData(), fileSize);
        return ao_load_data(buf, fileSize, libCallback);
    } else {
        g_FileContext = fi.absolutePath();
        wstring fileNameW = fi.absoluteFilePath().toStdWString();
        return ao_load_file(fileNameW.c_str(), libCallback);
    }
    return false;
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
