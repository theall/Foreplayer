
#include "../backendinterface.h"

const char *szName          = "Plugin display name";
const char *szManufacture   = "Your name/organization";
const char *szContact       = "Your contact information";
const char *szDescription   = "Plugin description";
const char *szCreateDate    = "Plugin created date, such as 2016-10-11";
const char *szSuffixes      = "";// for example "mp3 wma"

// Initialize plugin
EXPORT bool initialize()
{
    return true;
}

EXPORT const char *getLastError()
{
    return "";
}

// Verify this plugin can parse specify suffix of file
EXPORT const string matchSuffixes()
{
    // Return suffix list this plugin can process, multiple suffixed seperated by space character
    return szSuffixes;
}

// Parse file to get details information
EXPORT bool parse(char* file, TMusicInfo* musicInfo)
{
    // Parse file and fill result into musicInfo

    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    return true;
}

// Close track
EXPORT void closeTrack()
{

}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{

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
}
