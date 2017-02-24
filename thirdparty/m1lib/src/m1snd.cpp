/*
 * m1snd.cpp - main core for M1 sound player
 *
 */

#define SHOW_CLIPPING	(0)

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if SHOW_CLIPPING
#include <limits.h>
#endif
#include "m1snd.h"
#include "m1ui.h"
#include "m1queue.h"
#include "rom.h"
#include "oss.h"
#include "trklist.h"
#include "xmlout.h"
#include "gamelist.h"
#include "wavelog.h"

#define WORKRAM_SIZE (1024*1024)

#define M1SND_CORE_VERSION "0.7.9a1"

#define PLAYLISTS_ON_DEMAND	(1)

int g_initialized=0;
int wavelog = 0;
static int dsppos;
int boardtype;
static BoardT *g_curBoard = (BoardT *)NULL;
static int silencecount;
static int retrigger = 0, retriggernow = 0;
static int max = 0, min = 0;
static int bNormalize = 1;
static int b_pause;
static int b_albummode = 0, b_AllowList = 1;
static long songtime;
static int total_boards = 0;
static TimerT *shuftimer;

static struct M1Machine defmach;
static struct M1Driver2 defdrv;
struct M1Machine *Machine;
static int bDefcmd = 0, bUseInternalSnd = 1, bResetNormalizeBetweenSongs = 1;
static float fVolume = 1.0f, fPostVolume = 1.0f;
static float fHeadMix = 0.0f;

M1GameT *games = (M1GameT *)NULL;
static int total_games = 0;
static int cur_norm = 0;
static double framerate = 60.0;

unsigned int cmd1, albsong;

// use of these pointers is deprecated
unsigned char *prgrom, *prgrom2, *workram = NULL;

int curgame;

static int flushframes = 0;
static int samplerate = 44100;

int (STDCALL *m1ui_message)(void *m1this, int message, char *txt, int iparm);
void *m1ui_this;

static void audio_init(void);
//static void audio_kill(void);

#undef DRIVER
#define DRIVER( name ) &brd_##name,

static void *brdlist[] =
{
#include "drvlist.h"
	NULL
};

#define MAX_TRACKLISTS	(8192)

static TrkT *game_trklists[MAX_TRACKLISTS];

// run DSPs up to a certain position in the frame
void m1snd_update_dsps(long newpos, long totalsamp)
{
	int dspframes;

	dspframes = newpos - dsppos;

	// if not enough time elapsed to run any frames,
	// don't waste CPU calling all the DSPs
	if (dspframes <= 0) return;

	// make sure we don't overflow 1 frame
	if (dsppos >= totalsamp) return;

	// if this request is going to overflow, clip to 1 frame
	if ((dsppos + dspframes) >= totalsamp)
	{
		dspframes = totalsamp-dsppos;
	}

    if (g_curBoard && g_curBoard->UpdateDSPs)
	{
        g_curBoard->UpdateDSPs(dsppos, dspframes);
	}
	else
	{
		StreamSys_Update(dsppos, dspframes);
	}

	dsppos += dspframes;
}

//static int frSamples = 0;

DLLEXPORT void m1snd_do_frame(unsigned long dwSamples, signed short *out)
{
    if(!g_curBoard)
        return;

	int silent = -1;
//    frSamples++;
//    if(frSamples%60==0)
//        printf("%d\n", frSamples);

	dsppos = 0;
	timer_run_frame();

	m1snd_processQueue();	// process the command queue

	// finish up if necessary
	m1snd_update_dsps(dwSamples, dwSamples);

	// now call the "user handler"
    if (g_curBoard && g_curBoard->RunFrame)
	{
        g_curBoard->RunFrame();
	}

	StreamSys_Run(out, dwSamples);

	// see if we want to flush any frames
	if (flushframes)
	{
		memset(out, 0, dwSamples*4);
		flushframes--;
	}

	// let the UI do vu meters or whatever
//	m1ui_message(m1ui_this, M1_MSG_WAVEDATA, (char *)&out[0], dwSamples);

	if ((silent) && !m1snd_getQueueBootTime())
	{
		silencecount++;

		if (silencecount >= 3*60)
		{
			// tell the UI we're silent in case it wants to deal
			m1ui_message(m1ui_this, M1_MSG_SILENCE, NULL, 0);

			if (retrigger)
			{
				retriggernow = 1;
			}
			silencecount = 0;
		}
	}
	else
	{
		silencecount = 0;
	}

	songtime++;	// one more frame elapsed
}	

static void audio_init(void)
{
    if (!m1sdr_Init(samplerate, bUseInternalSnd))
    {
        m1ui_message(m1ui_this, M1_MSG_HARDWAREERROR, NULL, 0);
    }
    m1sdr_SetCallback((void *)m1snd_do_frame);
    //m1sdr_SetSamplesPerTick(samplerate/60);
	// init audio
}
#if 0
static void audio_kill(void)
{
	if (bUseInternalSnd)
	{
		m1sdr_PlayStop();
		m1sdr_Exit();
	}
}
#endif
void logerror(char *fmt,...)
{
	static char stmpbuf[512];
	va_list marker;

	va_start(marker,fmt);
	vsprintf(stmpbuf, fmt, marker );
	va_end(marker);

//	printf("%s", stmpbuf);
}

void syslog(char *fmt,...)
{
	static char stmpbuf[512];
	va_list marker;

	va_start(marker,fmt);
	vsprintf(stmpbuf, fmt, marker );
	va_end(marker);

//	printf("%s\n", stmpbuf);
}

char *getBoardName(int boardNumber)
{
    DrivertagsT *tboard = (DrivertagsT *)brdlist[boardNumber];
    char *pName = (char *)"unknown";
    if (!strncmp(tboard->id, "TAG0", 4))
    {
        int tag = 0;
        while (tboard->tags[tag].id != TID_INVALID)
        {
            if(tboard->tags[tag].id == TID_NAME)
            {
                pName = (char *)tboard->tags[tag].data0;
                break;
            }
            tag++;
        }
    }
    return pName;
}

char *getHardwareInfo(int boardNumber)
{
    DrivertagsT *tboard = (DrivertagsT *)brdlist[boardNumber];
    char *pName = (char*)"unknown";
    if (!strncmp(tboard->id, "TAG0", 4))
    {
        int tag = 0;
        while (tboard->tags[tag].id != TID_INVALID)
        {
            if(tboard->tags[tag].id == TID_HW)
            {
                pName = (char *)tboard->tags[tag].data0;
                break;
            }
            tag++;
        }
    }
    return pName;
}

// build a board structure from a taglist, or alternatively from
// a pointer to a literal board structure (deprecated)
static BoardT *construct_board(int num)
{
	DrivertagsT *tboard;
	int	tag, cpu, dsp;

	tboard = (DrivertagsT *)brdlist[num];

    BoardT *newBoard = (BoardT *)malloc(sizeof(BoardT));
    memset(newBoard, 0, sizeof(BoardT));

    newBoard->startupTime = 300;
    newBoard->interCmdTime = 100;

    Machine->refcon = games[curgame].refcon;

	if (!strncmp(tboard->id, "TAG0", 4))
	{
		// it's a tag board, construct
		tag = 0;
		cpu = -1;
		dsp = -1;
		while (tboard->tags[tag].id != TID_INVALID)
		{
/*			printf("Found tag ID %d data0 %x data1 %x data2 %x data3 %x\n",
		       		tboard->tags[tag].id,
		       		(unsigned int)tboard->tags[tag].data0,
		       		(unsigned int)tboard->tags[tag].data1,
		       		(unsigned int)tboard->tags[tag].data2,
		       		(unsigned int)tboard->tags[tag].data3);*/
			switch(tboard->tags[tag].id)
			{
				case TID_CPU:
					cpu++;
                    newBoard->bcpus[cpu].CPU_type = (long)tboard->tags[tag].data0;
                    newBoard->bcpus[cpu].speed = (long)tboard->tags[tag].data1;
//					printf("Adding CPU type %d speed %ld\n", cur_board->bcpus[cpu].CPU_type, cur_board->bcpus[cpu].speed);
					break;

				case TID_SOUND:
					dsp++;
                    newBoard->bmachine[dsp].sound_type = (long)tboard->tags[tag].data0;
                    newBoard->bmachine[dsp].sound_interface = tboard->tags[tag].data1;
//					printf("Adding DSP type %d intf %x\n", cur_board->bmachine[dsp].sound_type, (unsigned int)cur_board->bmachine[dsp].sound_interface);
					break;

				case TID_CPUMEMHAND:
                    newBoard->bcpus[cpu].memhandler = tboard->tags[tag].data0;
					break;

				case TID_CPURW:
					memory_cpu_rw(cpu, (void *)tboard->tags[tag].data0, (void *)tboard->tags[tag].data1);
					break;

				case TID_CPUOPREAD:
					memory_cpu_readop(cpu, (void *)tboard->tags[tag].data0);
					break;

				case TID_CPUPORTRW:
					memory_cpu_iorw(cpu, (void *)tboard->tags[tag].data0, (void *)tboard->tags[tag].data1);
					break;

				case TID_NAME:
                    newBoard->bname = (char *)tboard->tags[tag].data0;
//					printf("Board name is [%s]\n", cur_board->bname);
					break;

				case TID_HW:
                    newBoard->bhw = (char *)tboard->tags[tag].data0;
//					printf("Board hardware is [%s]\n", cur_board->bhw);
					break;

				case TID_DELAYS:
                    newBoard->startupTime = (long)tboard->tags[tag].data0;
                    newBoard->interCmdTime = (long)tboard->tags[tag].data1;
//					printf("Board delays are %ld %ld\n", cur_board->startupTime, cur_board->interCmdTime);
					break;

				case TID_INIT:
                    newBoard->InitBoard = (void (*)(long))tboard->tags[tag].data0;
					break;
				case TID_UPDATE:
                    newBoard->UpdateDSPs = (void (*)(long, long))tboard->tags[tag].data0;
					break;
				case TID_RUN:
                    newBoard->RunFrame = (void (*)(void))tboard->tags[tag].data0;
					break;
				case TID_SEND:
                    newBoard->SendCmd = (void (*)(int, int))tboard->tags[tag].data0;
					break;
				case TID_SHUTDOWN:
                    newBoard->Shutdown = (void (*)(void))tboard->tags[tag].data0;
					break;
				case TID_STOP:
                    newBoard->StopCmd = (void (*)(int))tboard->tags[tag].data0;
					break;

				default:
					m1ui_message(m1ui_this, M1_MSG_ERROR, (char *)"Error: driver using unknown tag ID!", 0);
					break;
			}

			tag++;
		}
	}
	else
	{
		// it's an old-style board
        memcpy(newBoard, brdlist[num], sizeof(BoardT));
	}

    return newBoard;
}

// initializes the board
static int start_board(void)
{
	// reset normalization and volume
	cur_norm = 0;

	// set the board type
	boardtype = games[curgame].btype;

	// reset the memory system (must happen before construct_board!)
	memory_boot();

	memset(workram, 0, WORKRAM_SIZE);

	// build the board structure
    g_curBoard = construct_board(boardtype);

	shuftimer = (TimerT *)NULL;

	m1snd_initQueue();

	m1ui_message(m1ui_this, M1_MSG_GAMENAME, games[curgame].name, 0);

	// load the roms
	if (!rom_loadgame())
	{
		m1ui_message(m1ui_this, M1_MSG_ROMLOADERR, NULL, 0);

		// clean up
		printf("Error loading ROMs\n");
		m1snd_run(M1_CMD_STOP, 0);
		return -1;
	}

	// (re)init the timer system
	timer_init(samplerate, framerate);
	timer_set_slices(1);

    Machine->drv->sound = g_curBoard->bmachine;
    Machine->drv->cpu = g_curBoard->bcpus;

	// init hardware
	prgrom  = rom_getregion(RGN_CPU1);
	prgrom2 = rom_getregion(RGN_CPU2);

	// streams want to be added at driver init, so do it now
	streams_sh_start();

    if (g_curBoard->InitBoard != NULL)
	{
        g_curBoard->InitBoard(samplerate);
	}

	// boot cpus after driver init in case of banking, etc.
	cpuintrf_boot();
	sound_boot();

	// reset CPUs now, as they may not have been able to read their
	// reset vectors during cpuintrf_boot().
	cpuintrf_reset();

	// output the board info to the UI
    m1ui_message(m1ui_this, M1_MSG_DRIVERNAME, g_curBoard->bname, 0);
    m1ui_message(m1ui_this, M1_MSG_HARDWAREDESC, g_curBoard->bhw, 0);
	
	// dispose the temp regions
	rom_postinit();

	// reset play time
	songtime = 0;

	return 0;
}

static void m1snd_initnewgame(void)
{
	waveLogInit(wavelog, games[curgame].zipname, cmd1);

	silencecount = 0;
	retriggernow = 0;

    //m1snd_startQueue();
    //m1snd_addToCmdQueue(cmd1);
    //m1snd_setQueueInitCmd(cmd1);
	
	m1snd_initNormalizeState();

	// check for fixed volume
	if (trklist_getfixvol(game_trklists[curgame]) != -1)
	{
		// don't override a user fixed volume
		if (fVolume == 1.0f)
		{
			fVolume = (float)trklist_getfixvol(game_trklists[curgame]);
			fVolume /= 100.0f;
		}
	}
	else
	{
		fVolume = 1.0f;
	}

	if (bUseInternalSnd)
	{
		m1sdr_PlayStart();
	}

}

DLLEXPORT void m1snd_switchLang(int lang)
{
    int i;
    for(i=0;i<MAX_TRACKLISTS;i++)
    {
        trklist_unload(game_trklists[i]);
        game_trklists[i] = (TrkT *)NULL;
    }
    trklist_setlang(lang);
    game_trklists[curgame] = trklist_load(games[curgame].zipname);
}

static int m1snd_switchgame(int newgame)
{
	curgame = newgame;

	if (bUseInternalSnd)
	{
		m1sdr_PlayStop();
	}

    if (g_curBoard)
	{
        if (g_curBoard->Shutdown)
		{
            g_curBoard->Shutdown();
		}
	
		sound_stop();

        free((void *)g_curBoard);
	}

    g_curBoard = (BoardT *)NULL;
	Machine->drv->sound = (MachineSound *)NULL;

	rom_shutdown();
	timer_clrnoturbo();
	timer_clear_cpus();
	memory_shutdown();
	if (start_board() < 0)
	{
		return 1;
	}

#if PLAYLISTS_ON_DEMAND
	if (!game_trklists[curgame])
	{
		game_trklists[curgame] = trklist_load(games[curgame].zipname);
	}
#endif

	// default to no album mode
	b_albummode = 0;
	if (!bDefcmd)
	{
		if ((trklist_getnumsongs(game_trklists[curgame])) && (b_AllowList))
		{
			// album mode, use .lst's default
			cmd1 = trklist_getdefcmd(game_trklists[curgame]);
			albsong = trklist_cmd2song(game_trklists[curgame], cmd1);
			b_albummode = 1;
		}
		else
		{
			cmd1 = games[curgame].defcmd;
			albsong = trklist_cmd2song(game_trklists[curgame], cmd1);
		}
	}
	else
	{
		if ((trklist_getnumsongs(game_trklists[curgame])) && (b_AllowList))
		{
			albsong = trklist_cmd2song(game_trklists[curgame], cmd1);
			b_albummode = 1;
		}
		bDefcmd = 0;
	}

	m1snd_initnewgame();

	return 0;
}

extern void initialise_decoder(void);

// called to init the m1 core
DLLEXPORT void m1snd_init(void *m1this, int (STDCALL *m1ui_msg)(void *,int, char *, int))                  
{
    if(g_initialized)
        return;
	int i;

	b_pause = 0;
	flushframes = 0;
	min = max = 0;

	// set the callback parameters
	m1ui_this = m1this;
	m1ui_message = m1ui_msg;

	// init fixed and post volumes
	fVolume = 1.0f;
	fPostVolume = 1.0f;

	// set up Machine structure
	Machine = &defmach;
	Machine->sample_rate = samplerate;
	Machine->drv = &defdrv;
	defdrv.frames_per_second = 60;

	audio_init();

    g_curBoard = (BoardT *)NULL;

	// find how many boards there are
	total_boards = 0;
	while (brdlist[total_boards] != NULL) 
	{
		total_boards++;
	}

	if (workram)
	{
		free(workram);
		workram = NULL;
	}

	workram = (unsigned char *)malloc(WORKRAM_SIZE);

	// load and construct the gamelist
	total_games = gamelist_load();

	// if no games were found, error out
	if (total_games == 0)
	{
		m1ui_message(m1ui_this, M1_MSG_ERROR, (char *)"Error: No games or couldn't find m1.xml!", 0);

		return;
	}

	if (total_games > MAX_TRACKLISTS)
	{
		printf("ERROR: MAX_TRACKLISTS is too small, should be %d\n", total_games);
	}

	// init tracklists to NULL
	for (i = 0; i < MAX_TRACKLISTS; i++)
	{
		game_trklists[i] = (TrkT *)NULL;
	}

#if !PLAYLISTS_ON_DEMAND
	for (i = 0; i < total_games; i++)
	{
//		printf("game %d: zipname = [%s]\n", i, games[i].zipname);
		game_trklists[i] = trklist_load(games[i].zipname);
#if 0
		if (game_trklists[i] != NULL)
		{
			xmlout_writelist(game_trklists[i], games[i].zipname);
		}
#endif
	}
#endif

#if 0
	// don't continue if the roms didn't load
	if (start_board() < 0)
	{
		return;
	}
	
	m1snd_initnewgame();
#endif

    g_initialized = 1;
}

// called on UI commands and/or during idle time to allow the audio to run
DLLEXPORT int m1snd_run(int command, int iparm)
{
	switch (command)
	{
		case M1_CMD_STOP:
            if (g_curBoard)
			{
                if (g_curBoard->Shutdown)
				{
                    g_curBoard->Shutdown();
				}
			
				sound_stop();
		
                free((void *)g_curBoard);
			}
		
            g_curBoard = (BoardT *)NULL;
			Machine->drv->sound = (MachineSound *)NULL;
			
			rom_shutdown();
			timer_clrnoturbo();
			timer_clear_cpus();
			break;

			// (intentional fallthrough)
		case M1_CMD_FLUSH:
			if (bUseInternalSnd)
			{
				m1sdr_FlushAudio();
				m1sdr_FlushAudio();
				m1sdr_FlushAudio();
				m1sdr_FlushAudio();
				m1sdr_FlushAudio();
				m1sdr_FlushAudio();
			}
			break;

		case M1_CMD_PAUSE:
			b_pause = 1;
			if (bUseInternalSnd)
			{
				m1sdr_Pause(1);
			}
			m1ui_message(m1ui_this, M1_MSG_PAUSE, (char *)NULL, 1);
			break;

		case M1_CMD_UNPAUSE:
			b_pause = 0;
			if (bUseInternalSnd)
			{
				m1sdr_Pause(0);
			}
			m1ui_message(m1ui_this, M1_MSG_PAUSE, (char *)NULL, 0);
			break;

		case M1_CMD_WRITELIST:
			xmlout_writelist(game_trklists[curgame], games[curgame].zipname);
			break;

		case M1_CMD_IDLE:	// do idle-time processing
            if (!g_curBoard)
			{
				return 0;
			}

			if (bUseInternalSnd)
			{
				m1sdr_TimeCheck();
			}

			if (retriggernow)
			{
                if (g_curBoard->StopCmd)
				{
                    g_curBoard->StopCmd(games[curgame].stopcmd);
				}
				else
				{
					if (games[curgame].stopcmd != -1)
					{
						m1snd_addToCmdQueue(games[curgame].stopcmd);
					}
				}
				m1snd_addToCmdQueue(cmd1);
				m1ui_message(m1ui_this, M1_MSG_STARTINGSONG, NULL, cmd1);

				retriggernow = 0;
			}
			break;

		case M1_CMD_GAMEJMP:	// jump to game #
			if (iparm < 0 || iparm >= total_games)
			{
				return 1;
			}

			// flush the audio.  let the driver decide how.
			if (bUseInternalSnd)
			{
				dsppos = 0;
				m1sdr_FlushAudio();
			}
			m1ui_message(m1ui_this, M1_MSG_FLUSH, (char *)NULL, 0);

			fPostVolume = 0.0f;

			curgame = iparm;

			m1ui_message(m1ui_this, M1_MSG_SWITCHING, games[curgame].name, 0);
			return m1snd_switchgame(curgame);
			break;

		case M1_CMD_SONGJMP:	// jump to song #
			fPostVolume = 0.0f;
			if (b_albummode)
			{
				if (iparm < 0 || iparm >= trklist_getnumsongs(game_trklists[curgame]))
				{
					return 1;
				}

				albsong = iparm;
				iparm = trklist_song2cmd(game_trklists[curgame], iparm);
			}
			else
			{
				if (iparm < 0 || iparm >= m1snd_get_info_int(M1_IINF_MAXSONG, 0))
				{
					return 1;
				}
			}

			if (!m1snd_isQueueBusy())
			{
				if (b_pause)
				{
					flushframes = 2;
				}

				waveLogStop();
				waveLogInit(wavelog, games[curgame].zipname, iparm);
				waveLogStart();

				cmd1 = iparm;
                if (g_curBoard)
				{
                    if (g_curBoard->StopCmd)
					{
                        g_curBoard->StopCmd(games[curgame].stopcmd);
					}
					else
					{
						if (games[curgame].stopcmd != -1)
						{
							m1snd_addToCmdQueue(games[curgame].stopcmd);
						}
					}
					m1snd_initNormalizeState();
					m1snd_addToCmdQueue(cmd1);
					m1ui_message(m1ui_this, M1_MSG_STARTINGSONG, NULL, cmd1);
					songtime = 0;
				}
			}
			else
			{
				return 1;
			}
			break;
	}

	return 0;	// success
}

// shuts down the m1 sound stuff
DLLEXPORT void m1snd_shutdown(void)
{
	int i, j;

	if (bUseInternalSnd)
	{
		m1sdr_PlayStop();
	}
	sound_stop();
	rom_shutdown();
	timer_clrnoturbo();
	timer_clear_cpus();

    if (g_curBoard)
	{
        if (g_curBoard->Shutdown)
		{
            g_curBoard->Shutdown();
		}

        free((void *)g_curBoard);
        g_curBoard = (BoardT *)NULL;
	}

	memory_shutdown();

	if (workram)
	{
		free(workram);
		workram = NULL;
	}

	if (bUseInternalSnd)
	{
		m1sdr_Exit();
	}

	for (i = 0; i < total_games; i++)
	{
		trklist_unload(game_trklists[i]);
		game_trklists[i] = (TrkT *)NULL;

//		free(games[i].parentzip);
//		free(games[i].zipname);
//		free(games[i].name);
//		free(games[i].year);
//		free(games[i].mfgstr);

		for (j = 0; j < ROM_MAX; j++)
		{
			if (games[i].roms[j].flags & ROM_ENDLIST)
			{
				j = ROM_MAX;
				continue;
			}

//			if (games[i].roms[j].flags & ROM_RGNDEF)
//			{
//				// nothing to do here
//			}
//			else
//			{
//				free(games[i].roms[j].name);
//			}
		}
	}

	if (games)
	{
		free(games);
		games = (M1GameT *)NULL;
	}
	z80_exit();

    g_initialized = 0;
}

// sets various options coming from the UI
DLLEXPORT void m1snd_setoption(int option, int value)
{
	switch (option)
	{
		// sets auto-retrigger mode
		case M1_OPT_RETRIGGER:
			retrigger = value;
			break;

		// sets the starting command #
		case M1_OPT_DEFCMD:
			cmd1 = value;
			bDefcmd = 1;
			break;

		// sets log to .WAV mode
		case M1_OPT_WAVELOG:
			wavelog = value;
			break;

		// turns normalization on and off
		case M1_OPT_NORMALIZE:
			bNormalize = 0;
			if (value) 
			{
				bNormalize = 1;
			}
			break;

		// set language for the track names
		case M1_OPT_LANGUAGE:
			trklist_setlang(value);
			break;

		// set if .lst/.xml files are used
		case M1_OPT_USELIST:
			b_AllowList = value;
			break;

		// set if M1 uses the system sound hardware
		case M1_OPT_INTERNALSND:
			bUseInternalSnd = value;
			break;

		case M1_OPT_SAMPLERATE:
			if (value < 8000)
			{
				value = 8000;
			}
			if (value > 48000)
			{
				value = 48000;
			}

			samplerate = value;
			framerate = 60.0;
			break;

		case M1_OPT_RESETNORMALIZE:
			bResetNormalizeBetweenSongs = value;
			break;

		case M1_OPT_FIXEDVOLUME:
			fVolume = (float)value;
			fVolume /= 100.0f;
			break;

		case M1_OPT_STEREOMIX:
			fHeadMix = (float)value;
			fHeadMix /= 100.0f;
			break;

		case M1_OPT_POSTVOLUME:
			if (value < 0)
			{
				value = 0;
			}
			if (value > 100)
			{
				value = 100;
			}
			fPostVolume = (float)value;
			fPostVolume /= 100.0f;
			break;

		case M1_OPT_SAMPLES_PER_FRAME:
			framerate = (double)samplerate / (double)value;
			break;
	}
}

// get numerical info about a game
DLLEXPORT int m1snd_get_info_int(int iinfo, int param)
{
	switch (iinfo)
	{
		case M1_IINF_HASPARENT:
            if ((param < 0) || (param > total_games))
			{
				return 0;
			}

            if (games[param].parentzip[0] != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			break;
		
		case M1_IINF_TOTALGAMES:
			return total_games;
			break;

		case M1_IINF_CURSONG:
			if (b_albummode)
			{
				return albsong;
			}

			return cmd1;
			break;

		case M1_IINF_CURCMD:
			return cmd1;
			break;

		case M1_IINF_CURGAME:
			return curgame;
			break;

		case M1_IINF_MINSONG:
			if (b_albummode)
			{
				return 0;
			}

			if ((games[curgame].mincmd == 0) && (games[curgame].maxcmd == 0))
			{
				// unknown settings, use old "AI"
				if (games[curgame].defcmd == 0) 
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return games[curgame].mincmd;
			}
			break;

		case M1_IINF_MAXSONG:
			if (b_albummode)
			{
				return trklist_getnumsongs(game_trklists[curgame]);
			}

			if (games[curgame].maxcmd == 0)
			{
				return 32768;
			}
			else
			{
				return games[curgame].maxcmd;
			}
			break;

		case M1_IINF_DEFSONG:
			if (b_albummode)
			{
				return 0;
			}

            return games[param].defcmd;
			break;

		case M1_IINF_MAXDRVS:
			return total_boards;
			break;

		case M1_IINF_BRDDRV:
            return games[param].btype;
			break;

		case M1_IINF_ROMSIZE:
            return rom_getfilesize(param&0xffff, param>>16);
			break;

		case M1_IINF_ROMCRC:
            return rom_getfilecrc(param&0xffff, param>>16);
			break;

		case M1_IINF_ROMNUM:
            return rom_getnum(param);
			break;

		case M1_IINF_TRACKS:
            if(!game_trklists[param])
                game_trklists[param] = trklist_load(games[param].zipname);
            return trklist_getnumsongs(game_trklists[param]);
			break;

		case M1_IINF_TRACKCMD:
            return trklist_song2cmd(game_trklists[param&0xffff], param>>16);
			break;

		case M1_IINF_TRKLENGTH:
            return trklist_getlength(game_trklists[param&0xffff], param>>16);
			break;

		case M1_IINF_CURTIME:
			return songtime;
			break;

		case M1_IINF_NUMEXTRAS:
            return trklist_getnumextras(game_trklists[param&0xffff], param>>16);
			break;

		case M1_IINF_NORMVOL:
			return cur_norm;
			break;

		case M1_IINF_NUMSTREAMS:
			return mixer_get_num_streams();
			break;

		case M1_IINF_NUMCHANS:
            return mixer_get_num_chans(param);
			break;

		case M1_IINF_CHANLEVEL:
            return mixer_get_chan_level(param>>16, param&0xffff);
			break;

		case M1_IINF_CHANPAN:
            return mixer_get_chan_pan(param>>16, param&0xffff);
			break;
	}

	return -1;
}

DLLEXPORT char *m1snd_get_info_str(int sinfo, int param)
{
	switch (sinfo)
	{
		case M1_SINF_ROMNAME:
            return games[param].zipname;
			break;

		case M1_SINF_YEAR:
            return games[param].year;
			break;

		case M1_SINF_VISNAME:
            return games[param].name;
			break;

		case M1_SINF_PARENTNAME:
            return games[param].parentzip;
			break;

		case M1_SINF_COREVERSION:
			return (char *)M1SND_CORE_VERSION;
			break;

		case M1_SINF_MAKER:
//			return mfgstrs[games[param].mfg];
            return games[param].mfgstr;
			break;

		case M1_SINF_BNAME:
            return getBoardName(param);
			break;

		case M1_SINF_BHARDWARE:
            return getHardwareInfo(param);
			break;

		case M1_SINF_ROMFNAME:
            return rom_getfilename(param&0xffff, param>>16);
			break;

		case M1_SINF_TRKNAME:
            return trklist_getname(game_trklists[param&0xffff], param>>16);
			break;

		case M1_SINF_ENCODING:
			return (char *)"UTF-8";
			break;

		case M1_SINF_CHANNAME:
            return mixer_get_chan_name(param>>16, param&0xffff);
			break;
	}

	return NULL;
}

DLLEXPORT char *m1snd_get_info_str_ex(int sinfo, int parm1, int parm2, int parm3)
{
	switch (sinfo)
	{
		case M1_SINF_EX_EXTRA:
			return trklist_getextra(game_trklists[parm1], parm2, parm3);
			break;
	}

	return NULL;
}

DLLEXPORT void m1snd_set_info_str(int sinfo, char *info, int parm1, int parm2, int parm3)
{
	switch (sinfo)
	{
		// parm1
		case M1_SINF_SET_TRKNAME:
			game_trklists[parm1] = trklist_setname(game_trklists[parm1], parm2, info);
			break;

		case M1_SINF_SET_EXTRA:
			game_trklists[parm1] = trklist_setextra(game_trklists[parm1], parm2, parm3, info);
			break;

		case M1_SINF_SET_ROMPATH:
			rom_setpath(info);
			break;

		case M1_SINF_SET_WAVPATH:
			waveLogSetPath(info);
			break;
	}
}

DLLEXPORT void m1snd_set_info_int(int iinfo, int parm1, int parm2, int parm3)
{
	switch (iinfo)
	{
		case M1_SIINF_CHANLEVEL:
			mixer_set_chan_level(parm1, parm2, parm3);
			break;

		case M1_SIINF_CHANPAN:
			mixer_set_chan_pan(parm1, parm2, parm3);
			break;
	}
}

void m1snd_initNormalizeState(void)
{
	// reset now, prevents volume anomolies when starting up/switching games
	if (!bResetNormalizeBetweenSongs)
	{
		min = max = cur_norm = 0;
	}
}

BoardT *m1snd_getCurBoard(void)
{
    return g_curBoard;
}

void m1snd_setPostVolume(float fPost)
{
	fPostVolume = fPost;
}

void m1snd_resetSilenceCount(void)
{
	silencecount = 0;
}

char *m1snd_get_custom_tag(const char *tagname)
{
	int i;

	for (i = 0; i < games[curgame].numcustomtags; i++) 
	{
		if (!strcasecmp(tagname, games[curgame].custom_tags[i].label))
		{
			return &games[curgame].custom_tags[i].value[0];
		}
	}
	
	return NULL;
}

UINT32 m1snd_get_custom_tag_value(const char *tagname)
{
	char *tagval;
	char *eptr;

	tagval = m1snd_get_custom_tag(tagname);

	if (tagval == NULL) 
	{
		return 0xffffffff;
	}

	return strtoul(tagval, &eptr, 0);
}

static char temp[1024];

char *cpuintrf_temp_str(void)
{
	return temp;
}
