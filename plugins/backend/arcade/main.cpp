
#include "m1thread.h"
#include "../backendinterface.h"

#include <QFile>
#include <QString>
#include <QLibrary>
#include <QFileInfo>
#include <QDataStream>

const char *szName = "Arcade";
const char *szManufacture = "bilge theall";
const char *szContact = "wazcd_1608@qq.com";
const char *szDescription = "This is the first backend plugin for foreplayer.";
const char *szCreateDate = "2016-10-11";

const char *szM1lib = "m1lib.dll";
unsigned char g_headerMark[4]=
{
    0x50,0x4b,0x03,0x04
};

QString szError;
QString g_curRomPath;
QLibrary g_library;
TM1Thread *g_runningThread;

M1SND_INIT proc_init;
M1SND_RUN proc_run;
M1SND_SHUTDOWN proc_shutdown;
M1SND_SETOPTION proc_setoption;
M1SND_GET_INFO_INT proc_get_info_int;
M1SND_GET_INFO_STR proc_get_info_str;
M1SND_GET_INFO_STR_EX proc_get_info_str_ex;
M1SND_SET_INFO_STR proc_set_info_str;
M1SND_SET_INFO_INT proc_set_info_int;
M1SND_DO_FRAME proc_do_frame;
M1SND_SWITCHLANG proc_switchlang;

// callbacks from the core of interest to the user interface
int m1ui_message(void *user, int message, char *txt, int iparam)
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
            curgame = proc_get_info_int(M1_IINF_CURGAME, 0);
            printf("Game selected: %s (%s, %s)\n", txt, proc_get_info_str(M1_SINF_MAKER, curgame), proc_get_info_str(M1_SINF_YEAR, curgame));
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
            proc_shutdown();
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
                    proc_set_info_str(M1_SINF_SET_ROMPATH, (char *)g_curRomPath.toLocal8Bit().constData(), 0, 0, 0);
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
    g_library.setFileName(szM1lib);

    if(!g_library.load())
    {
        szError = "Failed loading m1lib.dll";
        return false;
    }
    proc_init = NULL;
    proc_run = NULL;
    proc_shutdown = NULL;
    proc_setoption = NULL;
    proc_get_info_int = NULL;
    proc_get_info_str = NULL;
    proc_get_info_str_ex = NULL;
    proc_set_info_str = NULL;
    proc_set_info_int = NULL;
    proc_do_frame = NULL;
    proc_switchlang = NULL;

    proc_init = (M1SND_INIT)g_library.resolve("proc_init");
    if(proc_init==NULL)
    {
        szError = "Can not find procedure address [proc_init] in m1lib";
        return false;
    }
    proc_run = (M1SND_RUN)g_library.resolve("proc_run");
    if(proc_run==NULL)
    {
        szError = "Can not find procedure address [proc_run] in m1lib";
        return false;
    }
    proc_shutdown = (M1SND_SHUTDOWN)g_library.resolve("proc_shutdown");
    if(proc_shutdown==NULL)
    {
        szError = "Can not find procedure address [proc_shutdown] in m1lib";
        return false;
    }
    proc_setoption = (M1SND_SETOPTION)g_library.resolve("proc_setoption");
    if(proc_setoption==NULL)
    {
        szError = "Can not find procedure address [proc_setoption] in m1lib";
        return false;
    }
    proc_get_info_int = (M1SND_GET_INFO_INT)g_library.resolve("proc_get_info_int");
    if(proc_get_info_int==NULL)
    {
        szError = "Can not find procedure address [proc_get_info_int] in m1lib";
        return false;
    }
    proc_get_info_str = (M1SND_GET_INFO_STR)g_library.resolve("proc_get_info_str");
    if(proc_get_info_str==NULL)
    {
        szError = "Can not find procedure address [proc_get_info_str] in m1lib";
        return false;
    }
    proc_get_info_str_ex = (M1SND_GET_INFO_STR_EX)g_library.resolve("proc_get_info_str_ex");
    if(proc_get_info_str_ex==NULL)
    {
        szError = "Can not find procedure address [proc_get_info_str_ex] in m1lib";
        return false;
    }
    proc_set_info_str = (M1SND_SET_INFO_STR)g_library.resolve("proc_set_info_str");
    if(proc_set_info_str==NULL)
    {
        szError = "Can not find procedure address [proc_set_info_str] in m1lib";
        return false;
    }
    proc_set_info_int = (M1SND_SET_INFO_INT)g_library.resolve("proc_set_info_int");
    if(proc_set_info_int==NULL)
    {
        szError = "Can not find procedure address [proc_set_info_int] in m1lib";
        return false;
    }
    proc_do_frame = (M1SND_DO_FRAME)g_library.resolve("proc_do_frame");
    if(proc_do_frame==NULL)
    {
        szError = "Can not find procedure address [proc_do_frame] in m1lib";
        return false;
    }
    proc_switchlang = (M1SND_SWITCHLANG)g_library.resolve("proc_switchlang");
    if(proc_switchlang==NULL)
    {
        szError = "Can not find procedure address [proc_switchlang] in m1lib";
        return false;
    }

    if(!g_runningThread)
    {
        g_runningThread = new TM1Thread;
        QObject::connect(g_runningThread, &TM1Thread::finished, g_runningThread, &QObject::deleteLater);
        g_runningThread->start();
    }

    return true;
}

EXPORT const char *getLastError()
{
    return szError.toLocal8Bit().constData();
}

// Verify this plugin can parse specify suffix of file
EXPORT const string matchSuffixes()
{
    return "zip";
}

// Parse file to get details information
EXPORT bool parse(char* file, TMusicInfo* musicInfo)
{
    QFile musicFile(file);

    //If file is less than WMA header, it can't contains tag.
    if(musicFile.size()<16)
    {
        return false;
    }

    QDataStream musicDataStream(&musicFile);

    //Detect WMA header.
    char header[4];
    //Read header data from the music file.
    musicDataStream.readRawData(header, 4);
    //Compare the header data with the mark data.
    if(memcmp(header, g_headerMark, 4)!=0)
    {
        //The header is not right.
        return false;
    }

    int i;
    int maxGames = proc_get_info_int(M1_IINF_TOTALGAMES, 0);
    bool getOne = false;
    int gameId = 0;
    QString gameName = musicInfo->musicFileName.c_str();
    gameName.chop(4);

    for (i = 0; i < maxGames; i++)
    {
        if (gameName==proc_get_info_str(M1_SINF_ROMNAME, i))
        {
            getOne = true;

            //High 16 bits store game id.
            gameId = i;
            break;
        }
    }

    if(!getOne)
        return false;

    musicInfo->album = proc_get_info_str(M1_SINF_BNAME, gameId);
    musicInfo->artist = proc_get_info_str(M1_SINF_MAKER, gameId);
    musicInfo->date = proc_get_info_str(M1_SINF_YEAR, gameId);
    musicInfo->musicName = proc_get_info_str(M1_SINF_VISNAME, gameId);
    musicInfo->musicType = proc_get_info_str(M1_SINF_BNAME, gameId);
    musicInfo->musicTypeDesc = proc_get_info_str(M1_SINF_BHARDWARE, gameId);

    //Track list
    int trackCount = proc_get_info_int(M1_IINF_TRACKS, gameId);

    for (i = 0; i < trackCount; i++)
    {
        TTrackInfo *trackInfo = new TTrackInfo;
        trackInfo->index = (i<<16) | gameId;
        trackInfo->startPosition = 0;

        //Save the duration (unit: ms)
        trackInfo->duration = proc_get_info_int(M1_IINF_TRKLENGTH, i);
        //Save the bit rate.
        //Bitrate (Kbps)
        trackInfo->bitRate = 44;
        //Save the sampling rate.
        trackInfo->samplingRate = 44100;
        //File path.
        trackInfo->musicFullName = musicInfo->musicFullName;
        // TrackName
        trackInfo->trackName = proc_get_info_str(M1_SINF_TRKNAME, i);

        musicInfo->trackList.push_back(trackInfo);
    }

    //Mission complete.
    return true;
}

// Load track to prepare for playing
EXPORT bool loadTrack(TTrackInfo* trackInfo)
{
    closeTrack();

    QFileInfo fi(trackInfo->musicFullName.c_str());

    g_curRomPath = fi.absolutePath();

    int gameId = trackInfo->index&0xffff;
    if(gameId != proc_get_info_int(M1_IINF_CURGAME, 0))
    {
        proc_run(M1_CMD_GAMEJMP, gameId);
    }

    int songId = trackInfo->index>>16;
    proc_run(M1_CMD_SONGJMP, songId);

    qDebug("switching, game:%d song:%d", trackInfo->index&0xffff, trackInfo->index>>16);

    //Mission complete.
    return true;
}

// Close track
EXPORT void closeTrack()
{
    proc_run(M1_CMD_STOP, 0);
}

// Request next samples
EXPORT void nextSamples(int size, short* samples)
{
    proc_do_frame((unsigned long)size, (signed short*)samples);
}

// Retrieve plugin information
EXPORT void pluginInformation(TPluginInfo *pluginInfo)
{
    pluginInfo->name = szName;
    pluginInfo->manufacture = szManufacture;
    pluginInfo->contact = szContact;
    pluginInfo->description = szDescription;
    pluginInfo->createDate = szCreateDate;
}

// Use to free plugin
EXPORT void destroy()
{
    proc_shutdown();

    delete g_runningThread;
}
