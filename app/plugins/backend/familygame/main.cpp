#include <QMap>
#include <QString>
#include <QStringList>
#include <QFileInfo>

#include "fileparse.h"
#include "rarparse.h"
#include "zipparse.h"

const char *szName          = "family game";
const char *szManufacture   = "Your name/organization";
const char *szContact       = "Your contact information";
const char *szDescription   = "Plugin description";
const char *szCreateDate    = "";//Plugin created date, such as 2016-10-11
const char *g_typeDesc      = \
        "ZIP ;Universe package\n" \
        "RSN ;Super Nintendo package\n" \
        "AY  ;ZX Spectrum/Amstrad CPC\n" \
        "GBS ;Nintendo Game Boy\n" \
        "GYM ;Sega Genesis/Mega Drive\n" \
        "HES ;NEC TurboGrafx-16/PC Engine\n" \
        "KSS ;MSX Home Computer/other Z80 systems (doesn't support FM sound)\n" \
        "NSF ;Nintendo NES/Famicom (with VRC 6, Namco 106, and FME-7 sound)\n" \
        "SAP ;Atari systems using POKEY sound chip\n" \
        "SPC ;Super Nintendo/Super Famicom\n" \
        "VGM ;Sega Master System/Mark III, Sega Genesis/Mega Drive,BBC Micro\n";

TGmeWrap *g_gmePlay; // Gme for play
char g_szSuffixes[1024];
QMap<QString, QString> g_TypeDesc;

// Initialize plugin
EXPORT bool initialize()
{
    QString desc = g_typeDesc;
    QStringList itemList = desc.split("\n");
    foreach (QString s, itemList) {
        QStringList item = s.split(";");
        if(item.size() != 2)
            continue;

        QString suffix = item[0];
        suffix = suffix.trimmed().toLower();
        g_TypeDesc.insert(suffix, item[1]);
    }

    QByteArray suffixList = QString(g_TypeDesc.keys().join(" ")).toLocal8Bit();
    strcpy(g_szSuffixes, suffixList.constData());
    g_gmePlay = TGmeWrap::instance();

    return true;
}

EXPORT const char *getLastError()
{
    return "";
}

// Verify this plugin can parse specify suffix of file
EXPORT const char *matchSuffixes()
{
    // Return suffix list this plugin can process, multiple suffixed seperated by space character
    return (const char*)g_szSuffixes;
}

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
EXPORT const char *suffixDescription(const char *suffix)
{
    return g_TypeDesc.value(suffix).toLocal8Bit().constData();
}

// Parse file to get details information
EXPORT bool parse(const wchar_t *file, TMusicInfo* musicInfo)
{
    if(!file || !musicInfo)
        return false;

    // Parse file and fill result into musicInfo
    QString qFile = QString::fromWCharArray(file);
    QFileInfo fi(qFile);
    QString suffix = fi.suffix().toLower();
    bool result = false;
    TFileParse fileParse(qFile);
    TRarParse rarParse(qFile);
    TZipParse zipParse(qFile);
    if(g_TypeDesc.contains(suffix))
    {
        if(suffix=="rsn")
            result = rarParse.parse(musicInfo);
        else if(suffix=="zip")
            result = zipParse.parse(musicInfo);
        else
            result = fileParse.parse(musicInfo);
    } else {
        // For unknown suffix file, firstly try to verify whether it is a rsn file
        result = rarParse.parse(musicInfo);
        if(!result)
        {
            result = zipParse.parse(musicInfo);
            if(!result)
                result = fileParse.parse(musicInfo);
        }
    }
    return result;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    if(!g_gmePlay || !trackInfo)
        return false;

    bool error = false;
    QString qPath = QString::fromStdString(trackInfo->musicFileName);
    QFileInfo fi(qPath);
    QString suffix = fi.suffix().toLower();
    if(suffix=="rsn" || suffix=="rar")
    {
        // File is in package
        TRarParse rarParse(qPath);
        QByteArray trackData = rarParse.trackData(trackInfo);
        error = g_gmePlay->loadData(trackData.data(), trackData.size());
    } else if (suffix=="zip") {
        TZipParse zipParse(qPath);
        QByteArray trackData = zipParse.trackData(trackInfo);
        error = g_gmePlay->loadData(trackData.data(), trackData.size());
    } else {
        wstring wPath = qPath.toStdWString();
        error = g_gmePlay->loadFile(wPath.c_str());
    }
    if(error)
        error = g_gmePlay->startTrack(trackInfo->index);
    return error;
}

// Close track
EXPORT void closeTrack()
{
    g_gmePlay->free();
}

// Request next samples
EXPORT void nextSamples(int bufSize, byte* buffer)
{
    if(g_gmePlay)
        g_gmePlay->fillBuffer((short*)buffer, bufSize/2);
}

// Seek time
EXPORT bool seek(int microSeconds)
{
    if(g_gmePlay)
        return g_gmePlay->seek(microSeconds);

    return false;
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
    if(g_gmePlay) {
        TGmeWrap::deleteInstance();
        g_gmePlay = NULL;
    }
}
