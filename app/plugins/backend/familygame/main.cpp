#include <QMap>
#include <QString>
#include <QStringList>
#include <QFileInfo>

#include "fileparse.h"
#include "rarparse.h"

const char *szName          = "family game";
const char *szManufacture   = "Your name/organization";
const char *szContact       = "Your contact information";
const char *szDescription   = "Plugin description";
const char *szCreateDate    = "";//Plugin created date, such as 2016-10-11
const char *szTypeDesc      = \
        "RSN ;Super Nintendo package\n" \
        "AY  ;ZX Spectrum/Amstrad CPC\n" \
        "GBS ;Nintendo Game Boy\n" \
        "GYM ;Sega Genesis/Mega Drive\n" \
        "HES ;NEC TurboGrafx-16/PC Engine\n" \
        "KSS ;MSX Home Computer/other Z80 systems (doesn't support FM sound)\n" \
        "NSF ;Nintendo NES/Famicom (with VRC 6, Namco 106, and FME-7 sound)\n" \
        "SAP ;Atari systems using POKEY sound chip\n" \
        "SPC ;Super Nintendo/Super Famicom\n" \
        "VGZ ;Sega Master System/Mark III, Sega Genesis/Mega Drive,BBC Micro\n";

TGmeWrap *g_gmeWrap;
QString g_szSuffixes;
QMap<QString, QString> g_TypeDesc;

// Initialize plugin
EXPORT bool initialize()
{
    QString desc = szTypeDesc;
    QStringList itemList = desc.split("\n");
    foreach (QString s, itemList) {
        QStringList item = s.split(";");
        if(item.size() != 2)
            continue;

        QString suffix = item[0];
        suffix = suffix.trimmed().toLower();
        g_TypeDesc.insert(suffix, item[1]);
    }

    g_szSuffixes = g_TypeDesc.keys().join(" ");
    g_gmeWrap = TGmeWrap::instance();
    g_gmeWrap->init();

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
    return g_szSuffixes.toLocal8Bit().constData();
}

// Return description of this suffix, for example "mp3" should be "Moving Picture Experts Group Audio Layer III"
EXPORT const char *suffixDescription(const char *suffix)
{
    return g_TypeDesc.value(suffix).toLocal8Bit().constData();
}

// Parse file to get details information
EXPORT bool parse(const char *file, TMusicInfo* musicInfo)
{
    if(!file || !musicInfo)
        return false;

    // Parse file and fill result into musicInfo
    QFileInfo fi(file);
    QString suffix = fi.suffix().toLower();
    bool result = false;
    TFileParse fileParse(file);
    TRarParse rarParse(file);
    if(g_TypeDesc.contains(suffix))
    {
        if(suffix=="rsn")
            result = rarParse.parse(musicInfo);
        else
            result = fileParse.parse(musicInfo);
    } else {
        // For unknown suffix file, firstly try to verify whether it is a rsn file
        result = rarParse.parse(musicInfo);
        if(!result)
            result = fileParse.parse(musicInfo);
    }
    return result;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    if(!g_gmeWrap || !trackInfo)
        return false;

    bool error = false;
    const char *cPath = trackInfo->musicFileName.c_str();
    QFileInfo fi(cPath);
    QString suffix = fi.suffix().toLower();
    if(suffix=="rsn" || suffix=="rar")
    {
        // File is in package
        TRarParse rarParse(cPath);
        QByteArray trackData = rarParse.trackData(trackInfo);
        error = g_gmeWrap->loadData(trackData.data(), trackData.size());
    } else {
        error = g_gmeWrap->loadFile(cPath);
    }
    error = g_gmeWrap->startTrack(trackInfo->index);
    return error;
}

// Close track
EXPORT void closeTrack()
{
    g_gmeWrap->free();
}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{
    if(g_gmeWrap)
        g_gmeWrap->fillBuffer(samples, size);
}

// Seek time
EXPORT bool seek(int microSeconds)
{
    if(g_gmeWrap)
        return g_gmeWrap->seek(microSeconds);

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
    if(g_gmeWrap) {
        TGmeWrap::deleteInstance();
        g_gmeWrap = NULL;
    }
}
