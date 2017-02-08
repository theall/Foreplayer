
#include "m1thread.h"
#include "../backendinterface.h"

#include <QDir>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QLibrary>
#include <QFileInfo>
#include <QDataStream>

const char *szName = "Arcade";
const char *szManufacture = "bilge theall";
const char *szContact = "wazcd_1608@qq.com";
const char *szDescription = "This is the first backend plugin for foreplayer.";
const char *szCreateDate = "2016-10-11";

const char *szM1lib = "m1.dll";

QString szError;
QString g_curRomPath;
QLibrary g_library;
static TM1Thread *g_runningThread=NULL;

//M1SND_INIT m1snd_init;
//M1SND_RUN m1snd_run;
//M1SND_SHUTDOWN m1snd_shutdown;
//M1SND_SETOPTION m1snd_setoption;
//M1SND_GET_INFO_INT m1snd_get_info_int;
//M1SND_GET_INFO_STR m1snd_get_info_str;
//M1SND_GET_INFO_STR_EX m1snd_get_info_str_ex;
//M1SND_SET_INFO_STR m1snd_set_info_str;
//M1SND_SET_INFO_INT m1snd_set_info_int;
//M1SND_DO_FRAME m1snd_do_frame;
//M1SND_SWITCHLANG m1snd_switchlang;

// callbacks from the core of interest to the user interface
STDCALL int m1ui_message(void *user, int message, char *txt, int iparam)
{
    Q_UNUSED(user)
    Q_UNUSED(iparam)

    int curgame;

    switch (message)
    {
        // called when switching to a new game
        case M1_MSG_SWITCHING:
            printf("\nSwitching to game %s\n", txt);
            break;

        // called to show the current game's name
        case M1_MSG_GAMENAME:
            curgame = m1snd_get_info_int(M1_IINF_CURGAME, 0);
            printf("Game selected: %s (%s, %s)\n", txt, m1snd_get_info_str(M1_SINF_MAKER, curgame), m1snd_get_info_str(M1_SINF_YEAR, curgame));
            break;

        // called to show the driver's name
        case M1_MSG_DRIVERNAME:
            printf("Driver: %s\n", txt);
            break;

        // called to show the hardware description
        case M1_MSG_HARDWAREDESC:
            printf("Hardware: %s\n", txt);
            break;

        // called when ROM loading fails for a game
        case M1_MSG_ROMLOADERR:
            printf("ROM load error, bailing\n");
            //self->loadFailed();
            break;

        // called when a song is (re)triggered
        case M1_MSG_STARTINGSONG:
            break;

        // called if a hardware error occurs
        case M1_MSG_HARDWAREERROR:
            m1snd_shutdown();
            break;

        // called when the hardware begins booting
        case M1_MSG_BOOTING:
            printf("\nBooting hardware, please wait...");
            break;

        // called when the hardware is finished booting and is ready to play
        case M1_MSG_BOOTFINISHED:
            printf("ready!\n\n");
            break;

        // called when there's been at least 2 seconds of silence
        case M1_MSG_SILENCE:
            break;

        // called to let the UI do vu meters/spectrum analyzers/etc
        // txt = pointer to the frame's interleaved 16-bit stereo wave data
        // iparm = number of samples (bytes / 4)
        case M1_MSG_WAVEDATA:
            break;

        case M1_MSG_MATCHPATH:
            {
                if(!g_curRomPath.isEmpty())
                {
                    m1snd_set_info_str(M1_SINF_SET_ROMPATH, (char *)g_curRomPath.toLocal8Bit().constData(), 0, 0, 0);
                    return 1;
                }
                return 0;
            }
            break;

        case M1_MSG_GETWAVPATH:
            break;

        case M1_MSG_ERROR:
            printf("%s\n", txt);
            break;
    }

    return 0;
}

// Initialize plugin
EXPORT bool initialize()
{
    if(!g_runningThread)
    {
        g_runningThread = new TM1Thread;
        g_runningThread->start();
    }

    return true;
}

// Verify this plugin can parse specify suffix of file
EXPORT const char *matchSuffixes()
{
    return "zip";
}

// Parse file to get details information
EXPORT bool parse(const char *file, TMusicInfo* musicInfo)
{
    int i;
    int maxGames = m1snd_get_info_int(M1_IINF_TOTALGAMES, 0);
    bool getOne = false;
    int gameId = 0;
    QFileInfo fi(file);
    QString gameName = fi.baseName();
    for (i = 0; i < maxGames; i++)
    {
        if (gameName==m1snd_get_info_str(M1_SINF_ROMNAME, i))
        {
            getOne = true;

            //High 16 bits store game id.
            gameId = i;
            break;
        }
    }

    if(!getOne)
        return false;

    musicInfo->musicName = m1snd_get_info_str(M1_SINF_VISNAME, gameId);
    musicInfo->additionalInfo = QString::asprintf("Board: %s\n"
                                                  "Maker: %s\n"
                                                  "Year: %s\n"
                                                  "Hardware: %s",
                                                  m1snd_get_info_str(M1_SINF_BNAME, gameId),
                                                  m1snd_get_info_str(M1_SINF_MAKER, gameId),
                                                  m1snd_get_info_str(M1_SINF_YEAR, gameId),
                                                  m1snd_get_info_str(M1_SINF_BHARDWARE, gameId)
                                                  ).toStdString();

    //Track list
    int trackCount = m1snd_get_info_int(M1_IINF_TRACKS, gameId);

    for (i = 0; i < trackCount; i++)
    {
        TTrackInfo *trackInfo = new TTrackInfo;
        int trackCmdParam = (i<<16)|gameId;
        int trackCmd = m1snd_get_info_int(M1_IINF_TRACKCMD, trackCmdParam);
        int trackParam = (trackCmd<<16) | gameId;
        trackInfo->index = trackCmdParam;
        trackInfo->indexName = QString::asprintf("#%d", trackCmd).toStdString();

        //Save the duration (unit: ms)
        trackInfo->duration = m1snd_get_info_int(M1_IINF_TRKLENGTH, trackParam)*1000/60;

        // TrackName
        trackInfo->trackName = m1snd_get_info_str(M1_SINF_TRKNAME, trackParam);
        trackInfo->additionalInfo = musicInfo->additionalInfo;

        musicInfo->duration += trackInfo->duration;
        musicInfo->trackList.push_back(trackInfo);
    }

    //Mission complete.
    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    QFileInfo fi(trackInfo->musicFileName.c_str());

    g_curRomPath = fi.absoluteFilePath();

    int gameId = trackInfo->index&0xffff;
    if(gameId != m1snd_get_info_int(M1_IINF_CURGAME, 0))
    {
        m1snd_run(M1_CMD_GAMEJMP, gameId);
    }

    int songId = trackInfo->index>>16;
    m1snd_run(M1_CMD_SONGJMP, songId);

    qDebug("switching, game:%d song:%d", trackInfo->index&0xffff, trackInfo->index>>16);

    //Mission complete.
    return true;
}

// Close track
EXPORT void closeTrack()
{
    //m1snd_run(M1_CMD_STOP, 0);
}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{
    m1snd_do_frame((unsigned long)size/2, (signed short*)samples);
}

// Optional, for return customized sample size
EXPORT int sampleSize(int sampleRate, int fps)
{
    return (sampleRate*10+(fps>>1))/fps;
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
    m1snd_shutdown();
    if(g_runningThread) {
        g_runningThread->terminate();
        g_runningThread->wait();
        delete g_runningThread;
        g_runningThread = NULL;
    }
}
