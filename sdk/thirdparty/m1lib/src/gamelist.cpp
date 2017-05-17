/* --------------------------------
 * gamelist.cpp - game list support
 *
 */
#include  <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>

#include "m1snd.h"
#include "m1ui.h"

#include "board.h"

#include <expat.h>
#include "moddir.h"
#include "gamelist.h"

#define ROM_ENTRY_IS_REGION 0
#define ROM_ENTRY_IS_ROM    1

typedef unsigned char byte;

static char element_data[2048];
static int numgames, currgn;

typedef enum
{
	XML_INVALID = 0,
	XML_GOTM1,
	XML_GAME,
	XML_TITLE,
	XML_YEAR,
	XML_MAKER,
	XML_M1DATA,
	XML_REGION,
	XML_ROM,
	XML_COMPLETE,
} XMLStateE;

static XMLStateE xml_state;

static int rgn_from_name(char *name)
{
    static char *rnames[RGN_MAX_COUNT] =
	{
		(char *)"cpu1", (char *)"cpu2", (char *)"cpu3", (char *)"cpu4", (char *)"samp1", (char *)"samp2", 
		(char *)"samp3", (char *)"samp4", (char *)"user1", (char *)"user2", (char *)"disk"
	};
    static int rnums[RGN_MAX_COUNT] =
	{
		RGN_CPU1, RGN_CPU2, RGN_CPU3, RGN_CPU4, RGN_SAMP1, RGN_SAMP2, RGN_SAMP3, RGN_SAMP4, RGN_USER1, RGN_USER2, RGN_DISK
	};
	int i;

    for (i = 0; i < RGN_MAX_COUNT; i++)
	{
		if (!strcmp(rnames[i], name))
		{
			return rnums[i];
		}
	}

	printf("ERROR: unable to find region [%s]\n", name);
	return 0;
}

static int find_board_by_name(char *name)
{
	int maxboard = m1snd_get_info_int(M1_IINF_MAXDRVS, 0);
	int i;

	for (i = 0; i < maxboard; i++)
	{
		if (!strcmp(m1snd_get_info_str(M1_SINF_BNAME, i), name))
		{
			return i;
		}
	}

	printf("ERROR: unable to find board [%s]\n", name);
	return 0;
}

static void startElement(void *userData, const char *name, const char **atts)
{
	int i, val;
	int *depthPtr = (int *)userData;
	char *eptr;

	element_data[0] = '\0';

	if (!strcmp(name, "m1"))		xml_state = XML_GOTM1;
	else if (!strcmp(name, "description"))	xml_state = XML_TITLE;
	else if (!strcmp(name, "year"))		xml_state = XML_YEAR;
	else if (!strcmp(name, "manufacturer"))	xml_state = XML_MAKER;
	else if (!strcmp(name, "m1data"))	
	{
		xml_state = XML_M1DATA;
		games[curgame].numcustomtags = 0;	
	}
	else if (!strcmp(name, "game"))
	{
		// bump up both the current game number and the number of games
		curgame++;
		numgames++;

		// (re)size the gamelist to fit the new number of games
		games = (M1GameT *)realloc(games, sizeof(M1GameT)*numgames);

		// zero out the newly allocated entry
		memset(&games[curgame], 0, sizeof(M1GameT));

		// parentzip must exist even if no element for it is present (for back compatibility),
		// so allocate it now
        //games[curgame].parentzip = (char *)malloc(16);
		games[curgame].parentzip[0] = '\0';

		// and set the state
		xml_state = XML_GAME;

		// reset the region pointer too
		currgn = -1;
	}
	else if ((!strcmp(name, "rom")) || (!strcmp(name, "disk")))	// we'll accept both
	{
		xml_state = XML_ROM;

		if (currgn < (ROM_MAX-1))
		{
			currgn++; 
		}

		// clear our flags
		games[curgame].roms[currgn].flags = 0;

		// mark the end of the list now (the clear flags step above will take care of it if we get there)
		games[curgame].roms[currgn+1].flags = ROM_ENDLIST;
	}
	else if (!strcmp(name, "region"))	
	{ 
		xml_state = XML_REGION; 
		
		if (currgn < (ROM_MAX-1))
		{
			currgn++; 
		}

		// clear our flags
		games[curgame].roms[currgn].flags = 0;

		// mark the end of the list now (the clear flags step above will take care of it if we get there)
		games[curgame].roms[currgn+1].flags = ROM_ENDLIST;
	};

	i = 0;
	while (atts[i] != (const char *)NULL)
	{
//		printf("%s = [%s]\n", atts[i], atts[i+1]);
		switch (xml_state)
		{
			case XML_GOTM1:
				if (!strcmp(atts[i], "version"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					if (val != 1)
					{
						printf("Unknown M1.XML file version.  M1 may malfunction!\n");
					}
/*					else
					{
						printf("XML version OK\n");
					}*/
				}
				break;

			case XML_GAME:
				if (!strcmp(atts[i], "name"))
				{
					games[curgame].zipname = (char *)malloc(strlen(atts[i+1])+1);
					strcpy(games[curgame].zipname, atts[i+1]);
				}
				else if (!strcmp(atts[i], "board"))
				{
					games[curgame].btype = find_board_by_name((char *)atts[i+1]);
				}
				else if (!strcmp(atts[i], "romof"))
				{
					strcpy(games[curgame].parentzip, atts[i+1]);
				}
				break;

			case XML_M1DATA:
				if (!strcmp(atts[i], "default"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].defcmd = val;
				}
				else if (!strcmp(atts[i], "stop"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].stopcmd = val;
				}
				else if (!strcmp(atts[i], "min"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].mincmd = val;
				}
				else if (!strcmp(atts[i], "max"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].maxcmd = val;
				}
				else if (!strcmp(atts[i], "subtype"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].refcon = val;
				}
				else
				{
//					printf("custom tag %d: %s = %s\n", games[curgame].numcustomtags, atts[i], atts[i+1]);
					strncpy(games[curgame].custom_tags[games[curgame].numcustomtags].label, atts[i], 31);
					strncpy(games[curgame].custom_tags[games[curgame].numcustomtags].value, atts[i+1], 31);
					if (games[curgame].numcustomtags < MAX_CUSTOM_TAGS) 
					{
						games[curgame].numcustomtags++;
					}
					else
					{
						printf("Ran out of custom tags for game, only %d allowed\n", MAX_CUSTOM_TAGS);
					}
				}
				break;

			case XML_REGION:
				if (!strcmp(atts[i], "type"))
				{
					games[curgame].roms[currgn].loadadr = rgn_from_name((char *)atts[i+1]);
					games[curgame].roms[currgn].flags |= ROM_RGNDEF;
//					printf("ROM %d: REGION type %s = %ld\n", currgn, atts[i+1], games[curgame].roms[currgn].loadadr);
				}
				else if (!strcmp(atts[i], "size"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].roms[currgn].length = val;
				}
				else if (!strcmp(atts[i], "clear"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].roms[currgn].flags |= (val & 0xff)<<8;
					games[curgame].roms[currgn].flags |= RGN_CLEAR;
				}
				else if (!strcmp(atts[i], "endian"))
				{
					if (atts[i+1][0] == 'b')
					{
						games[curgame].roms[currgn].flags |= RGN_BE;
					}
					else
					{
						games[curgame].roms[currgn].flags |= RGN_LE;
					}
				}
				else printf("UNK REGION attr [%s]\n", atts[i]);
				break;

			case XML_ROM:
				if (!strcmp(atts[i], "name"))
				{
//					printf("ROM %d: ROM type %s\n", currgn, atts[i+1]);
					games[curgame].roms[currgn].name = (char *)malloc(strlen(atts[i+1])+1);
					strcpy(games[curgame].roms[currgn].name, atts[i+1]);
				}
				else if (!strcmp(atts[i], "size"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].roms[currgn].length = val;
				}
				else if (!strcmp(atts[i], "crc"))
				{
					val = strtoul(atts[i+1], &eptr, 16);
					games[curgame].roms[currgn].crc = val;
				}
				else if (!strcmp(atts[i], "offset"))
				{
					val = strtoul(atts[i+1], &eptr, 16);
					games[curgame].roms[currgn].loadadr = val;
				}
				else if (!strcmp(atts[i], "flip"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].roms[currgn].flags |= ROM_REVERSE;
				}
				else if (!strcmp(atts[i], "skip"))
				{
					val = strtoul(atts[i+1], &eptr, 0);
					games[curgame].roms[currgn].flags |= val<<12;
				}
				else if (!strcmp(atts[i], "width"))
				{
					if (atts[i+1][0] == 'd')
					{
						games[curgame].roms[currgn].flags |= ROM_DWORD;
					}
					else if (atts[i+1][0] == 'w')
					{
						games[curgame].roms[currgn].flags |= ROM_WORD;
					}
				}
				else if (!strcmp(atts[i], "sha1"))
				{
					strcpy(games[curgame].roms[currgn].sha1, atts[i+1]);
				}
				else if (!strcmp(atts[i], "md5"))
				{
					strcpy(games[curgame].roms[currgn].md5, atts[i+1]);
				}
//				else printf("UNK ROM attr [%s]\n", atts[i]);
				break;

			default:
				break;
		}
		i+=2;
	}

	*depthPtr += 1;
}

static void endElement(void *userData, const char *name)
{
    UNUSED(name);

    int *depthPtr = (int *)userData;
	*depthPtr -= 1;

//	printf("close [%s]: element data = [%s]\n", name, element_data);

	switch (xml_state)
	{
		case XML_TITLE:
			games[curgame].name = (char *)malloc(strlen(element_data)+1);
			strcpy(games[curgame].name, element_data);
			break;

		case XML_YEAR:
            //games[curgame].year = (char *)malloc(4+1);
			strncpy(games[curgame].year, element_data, 4);
			games[curgame].year[4] = '\0';
			break;

		case XML_MAKER:
			games[curgame].mfgstr = (char *)malloc(strlen(element_data)+1);
			strcpy(games[curgame].mfgstr, element_data);
			break;

		default:
			break;
	}

	element_data[0] = '\0';
}

static void charData(void *userData, const char *s, int len)
{
    UNUSED(userData);

	strncat(element_data, s, len);
}

// just pass through all 8-bit values for now
static int unkEncodingHandler(void *handlerData, const XML_Char *name, XML_Encoding *info)
{
    UNUSED(handlerData);
    UNUSED(name);

	int i;

	for (i = 0; i < 256; i++)
	{
		info->map[i] = i;
	}
	info->convert = NULL;
	info->release = NULL;

	return XML_STATUS_OK;
}

#include <time.h>
int loadFromXml(wchar_t *xmlFile)
{
    XML_Parser parser = XML_ParserCreate(NULL);
    FILE *f;
    int depth = 0, done;
    static char buf[BUFSIZ];

    curgame = -1;
    numgames = 0;
    games = (M1GameT *)NULL;	// realloc() will become malloc() if ptr is NULL, which is handy

    f = _wfopen(xmlFile, L"r");
    if (!f)
    {
        return 0;
    }

    XML_SetUserData(parser, &depth);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, charData);
    XML_SetUnknownEncodingHandler(parser, unkEncodingHandler, NULL);

    xml_state = XML_INVALID;

    do
    {
        unsigned int len = fread(buf, 1, sizeof(buf), f);

        done = (len < sizeof(buf));

        if (!XML_Parse(parser, buf, len, done))
        {
            printf("XML parse error %s at line %d (m1.xml)\n",
                XML_ErrorString(XML_GetErrorCode(parser)),
                (int)XML_GetCurrentLineNumber(parser));

            // clean up
            XML_ParserFree(parser);
            if (games)
            {
                free(games);
                games = (M1GameT *)NULL;
            }
            return 0;
        }

    } while (!done);

    XML_ParserFree(parser);

    fclose(f);
    return numgames;
}

/**
 *Functions for read from binary
 */
void readString(byte *&p, char **dest)
{
    *dest = (char*)p;
    p += strlen(*dest) + 1;
}

void readInt(byte *&p, int *dest)
{
    *dest = *(int*)p;
    p += sizeof(int);
}

void readLong(byte *&p, long *dest)
{
    *dest = *(long*)p;
    p += sizeof(long);
}

void readM1Data(byte *&p, M1GameT *game)
{
    readInt(p, &game->defcmd);
    readInt(p, &game->stopcmd);
    readInt(p, &game->mincmd);
    readInt(p, &game->maxcmd);
    readInt(p, &game->refcon);
    readInt(p, &game->numcustomtags);
    if(game->numcustomtags > 0)
    {
        int realTags = std::min(game->numcustomtags, MAX_CUSTOM_TAGS);
        int unitSize = sizeof(BoardOptionsT);
        int needMoved = std::max(0, (game->numcustomtags-MAX_CUSTOM_TAGS)*unitSize);
        int i;

        for(i=0;i<realTags;i++)
        {
            memcpy(&game->custom_tags[i], p, unitSize);
            p += unitSize;
        }
        if(needMoved > 0)
        {
            printf("Ran out of custom tags for game, only %d allowed\n", MAX_CUSTOM_TAGS);
            p += needMoved;
        }
    }
}

void readRom(byte *&p, RomEntryT *rom)
{
    readString(p, &rom->name);
    readInt(p, (int*)&rom->length);
    readInt(p, (int*)&rom->crc);
    readInt(p, (int*)&rom->loadadr);
    if(*p++)
        rom->flags |= ROM_REVERSE;

    char skip = *p++;
    if(skip != -1)
        rom->flags |= int(skip)<<12;

    char w = *p++;
    if(w == 4)
        rom->flags |= ROM_DWORD;
    else if(w == 2)
        rom->flags |= ROM_WORD;

    // Check sha1
    if(*p++)
    {
        memcpy(rom->sha1, p, 40);
        p += 40;// Sha1 length
    }

    // Check md5
    if(*p++)
    {
        memcpy(rom->md5, p, 32);
        p += 32;// md5 length
    }
}

void readRegion(byte *&p, RomEntryT *rom)
{
    readInt(p, (int*)&rom->loadadr);
    if(rom->loadadr != -1)
        rom->flags |= ROM_RGNDEF;

    readInt(p, (int*)&rom->length);
    int clear = 0;
    readInt(p, &clear);
    if(clear != -1)
    {
        rom->flags |= (clear & 0xff)<<8;
        rom->flags |= RGN_CLEAR;
    }
    if(*p++ == 0)
        rom->flags |= RGN_BE;
    else
        rom->flags |= RGN_LE;
}

void readRegions(byte *&p, M1GameT *game)
{
    int regions = 0;
    readInt(p, &regions);
    int i;
    for(i=0;i<regions;i++)
    {
        if(*p++==ROM_ENTRY_IS_REGION)
            readRegion(p, &game->roms[i]);
        else
            readRom(p, &game->roms[i]);
    }
    game->roms[i].flags = ROM_ENDLIST;
}

void readGame(byte *&p, M1GameT *game)
{
    readString(p, &game->zipname);
    readInt(p, &game->btype);
    readString(p, &game->name);
    memcpy(game->year, p, 4);
    p += 5;
    readString(p, &game->mfgstr);
    readM1Data(p, game);
    readRegions(p, game);
}

int loadFromBin(wchar_t *fileName)
{
    FILE *fp = _wfopen(fileName, L"rb");
    if (!fp)
        return 0;
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    if(fileSize <= 0)
    {
        fclose(fp);
        return 0;
    }
    byte *fileBuf = (byte*)malloc(fileSize);
    byte *pBuf = fileBuf;
    fseek(fp, 0, SEEK_SET);
    int sizeRead = fread(fileBuf, 1, fileSize, fp);
    fclose(fp);
    if(sizeRead < fileSize)
        return 0;

    int gameCount = *((int*)pBuf);
    pBuf += sizeof(int);
    if(gameCount <= 0)
        return 0;

    int i;
    if(games)
        games = (M1GameT *)realloc(games, sizeof(M1GameT)*gameCount);
    else
        games = (M1GameT *)malloc(sizeof(M1GameT)*gameCount);
    memset(games, 0, sizeof(M1GameT)*gameCount);
    for(i=0;i<gameCount;i++)
        readGame(pBuf, &games[i]);

    //free(fileBuf);

    numgames = gameCount;
    return gameCount;
}

int gamelist_load()
{
    wchar_t xmlPath[280];
    wchar_t binPath[280];
    wchar_t *szFilePath = currentModulePath();
    wcscpy(xmlPath, szFilePath);
    wcscat(xmlPath, L"m1.xml");
    wcscpy(binPath, szFilePath);
    wcscat(binPath, L"m1.bin");
    int n = 0;
#if M1_DEBUG
    clock_t timeStart = clock();
#endif
    if(_waccess(binPath, 0)==-1)
    {
        n = loadFromXml(xmlPath);
        if(n > 0)
            n = gamelist_save(binPath);
    } else {
        n = loadFromBin(binPath);
//        if(n <= 0)
//            n = loadFromXml(xmlPath);
    }

//    wchar_t suffix[280];
//    int l = wcslen(fileName);
//    wchar_t *p = fileName + l;
//    while(*p!=L'.' && p>=fileName)
//        p--;
//    p++;
//    wcscpy(suffix, p);
//    wcslwr(suffix);
//    if(!wcscmp(suffix, L"xml"))
//        return loadFromXml(currentDllPath);
//    else
//        return loadFromBin(currentDllPath);
#if M1_DEBUG
    printf("Gamelist parse time: %ldms\n", (clock()-timeStart));
#endif
    return n;
}

/**
 * @brief gamelist_save
 * @param fileName
 * @return
 */

void writeByte(FILE *fp, byte c)
{
    fwrite(&c, sizeof(byte), 1, fp);
}

void writeInt(FILE *fp, int i)
{
    fwrite(&i, sizeof(int), 1, fp);
}

void writeString(FILE *fp, const char *str)
{
    fwrite(str, 1, strlen(str), fp);
    writeByte(fp, 0);
}

void writeM1Data(FILE *fp, M1GameT *game)
{
    writeInt(fp, game->defcmd);
    writeInt(fp, game->stopcmd);
    writeInt(fp, game->mincmd);
    writeInt(fp, game->maxcmd);
    writeInt(fp, game->refcon);

    if(game->numcustomtags > MAX_CUSTOM_TAGS)
        game->numcustomtags = MAX_CUSTOM_TAGS;

    writeInt(fp, game->numcustomtags);
    if(game->numcustomtags > 0)
    {
        fwrite(game->custom_tags, sizeof(BoardOptionsT), game->numcustomtags, fp);
    }
}

void writeRom(FILE *fp, RomEntryT *rom)
{
    writeString(fp, rom->name);
    writeInt(fp, rom->length);
    writeInt(fp, rom->crc);
    writeInt(fp, rom->loadadr);

    if(rom->flags&ROM_REVERSE)
        writeByte(fp, 1);
    else
        writeByte(fp, 0);

    byte skip = (rom->flags>>12)&0x0f;
    if(skip > 0)
        writeByte(fp, skip);
    else
        writeByte(fp, -1);

    if(rom->flags & ROM_DWORD)
        writeByte(fp, 4);
    else if(rom->flags & ROM_WORD)
        writeByte(fp, 2);
    else
        writeByte(fp, -1);

    // Check sha1
    if(rom->sha1[0]=='\0')
        writeByte(fp, 0);
    else
    {
        writeByte(fp, 1);
        fwrite(rom->sha1, 1, 40, fp);
    }

    // Check md5
    if(rom->md5[0]=='\0')
        writeByte(fp, 0);
    else
    {
        writeByte(fp, 1);
        fwrite(rom->md5, 1, 32, fp);
    }
}

void writeRegion(FILE *fp, RomEntryT *rom)
{
    if(rom->flags & ROM_RGNDEF)
        writeInt(fp, (int)rom->loadadr);
    else
        writeInt(fp, -1);

    writeInt(fp, (int)rom->length);

    if(rom->flags & RGN_CLEAR)
    {
        int clear = (rom->flags&0xff00)>>8;
        writeInt(fp, clear);
    } else {
        writeInt(fp, -1);
    }
    if(rom->flags & RGN_BE)
        writeByte(fp, 0);
    else
        writeByte(fp, 1);
}

void writeRomEntry(FILE *fp, RomEntryT *rom)
{
    if(rom->name==NULL)
    {
        writeByte(fp, ROM_ENTRY_IS_REGION);
        writeRegion(fp, rom);
    } else {
        writeByte(fp, ROM_ENTRY_IS_ROM);
        writeRom(fp, rom);
    }
}

void writeRegions(FILE *fp, M1GameT *game)
{
    int roms = 0;
    while(roms <= ROM_MAX)
    {
        if(game->roms[roms].name==NULL && game->roms[roms].length==0)
        {
            break;
        }
        roms++;
    }
    if(roms <= ROM_MAX)
    {
        writeInt(fp, roms);
        for(int i=0;i<roms;i++)
            writeRomEntry(fp, &game->roms[i]);
    } else {
        writeInt(fp, 0);
    }
}

void writeGame(FILE *fp, M1GameT *game)
{
    writeString(fp, game->zipname);
    writeInt(fp, game->btype);
    writeString(fp, game->name);
    writeString(fp, game->year);
    writeString(fp, game->mfgstr);
    writeM1Data(fp, game);
    writeRegions(fp, game);
}

int gamelist_save(wchar_t *fileName)
{
    if(!games || !numgames)
        return 0;

    FILE *fp = _wfopen(fileName, L"wb");
    if(!fp)
        return 0;
    int i;
    writeInt(fp, numgames);
    for(i=0;i<numgames;i++)
    {
        writeGame(fp, &games[i]);
    }
    fclose(fp);

    return numgames;
}
