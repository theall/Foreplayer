#include "fileparse.h"

#include <QString>
#include <QFileInfo>
#include <QStringList>
#include <QByteArray>

#include "math.h"

struct TAG
{
    uint id;
    char *desc;
};

static TAG g_ValidFid[] =
{
    { 0x50534641, (char*)"Capcom QSound" },
    { 0x50534611, (char*)"Sega Saturn" },
    { 0x50534601, (char*)"Sony PlayStation" },
    { 0x53505500, (char*)"Sony PlayStation" },
    { 0x50534602, (char*)"Sony PlayStation 2" },
    { 0x50534612, (char*)"Sega Dreamcast" },

    { 0xffffffff, NULL }
};

bool isValidFile(char *tag)
{
    uint id = (uint)tag[0]<<24|(uint)tag[1]<<16|(uint)tag[2]<<8|(uint)tag[3];
    int i = 0;
    while(true)
    {
        if(g_ValidFid[i].id==id || g_ValidFid[i].id==0xffffffff)
            break;
        i++;
    }
    return g_ValidFid[i].id!=0xffffffff;
}

unsigned int toMicroSeconds(const char *timeStr)
{
    if(!timeStr)
        return 0;

    char tm[3][3];
    memset(tm, 0, 9);

    char *pt = (char*)timeStr;
    int len = strlen(timeStr);
    int iArray = 0;
    int aIndex = 0;
    int i;
    if(len>11)
        return 0;

    int msecs = 0;
    for(i=0;i<len;i++)
    {
        char c = *pt;
        if(c==':')
        {
            tm[iArray][aIndex] = 0;
            aIndex = 0;
            iArray++;
            pt++;
            continue;
        } else if(c=='.') {
            tm[iArray][aIndex] = 0;
            msecs = atoi(pt+1);
            break;
        }
        tm[iArray][aIndex] = c;
        aIndex++;
        pt++;
    }
    iArray++;

    int r = 3 - iArray;
    int microSecs = 0;
    i = 0;
    while(i<3)
    {
        microSecs += atoi(tm[i])*pow(60, 2-i-r);
        i++;
    }
    return microSecs*1000+msecs;
}

void fillTrackInfo(QString s, TTrackInfo *info)
{
    QStringList sl = s.split("\n");
    if(!sl.isEmpty())
    {
        QStringList otherItems;
        foreach (QString item, sl) {
            QStringList keyValue = item.split("=");
            if(keyValue.size() != 2)
                continue;

            QString key = keyValue[0].toLower();
            std::string value = keyValue[1].toStdString();
            if(key=="title")
            {
                info->trackName = value;
            } else if (key=="length") {
                info->duration = toMicroSeconds(value.c_str());
            }  else if (key=="game") {
                info->game = value;
            } else if (key=="year") {
                info->year = keyValue[1].toInt();
            } else if (key=="artist") {
                info->artist = value;
            } else {
                otherItems.append(item);
            }
        }
        QString otherString = otherItems.join("\n");
        otherString.replace("=", ": ");
        info->additionalInfo = otherString.toStdString();
    }
}

bool getMusicInfo(const wchar_t *file, TTrackInfo *info)
{
    FILE *fp = _wfopen(file, L"rb");
    if(!fp)
        return false;

    int header[4];// 4 ints: id, res_area, rom size, crc
    int headerSize = 16;
    fread(&header, 4, 4, fp);
    if(!isValidFile((char*)&header[0]))
    {
        fclose(fp);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    int tagOffset = headerSize + header[1] + header[2];
    if(tagOffset>=fileSize)
    {
        fclose(fp);
        return false;
    }
    fseek(fp, tagOffset, SEEK_SET);
    int sizeNeed = fileSize-tagOffset;
    char *buf = (char*)malloc(sizeNeed+1);
    memset(buf, 0, sizeNeed+1);
    int sizeRead = fread(buf, 1, sizeNeed, fp);
    fclose(fp);
    if(sizeRead != sizeNeed || strncmp(buf, "[TAG]", 5) != 0)
        return false;

    QString s = buf+5;
    free(buf);
    fillTrackInfo(s, info);
    return true;
}

bool getMusicInfo(QByteArray data, TTrackInfo *info)
{
    char *pBuf = data.data();

    int header[4];// 4 ints: id, res_area, rom size, crc
    int headerSize = 16;
    memcpy(&header, pBuf, headerSize);
    if(!isValidFile((char*)&header[0]))
        return false;

    int fileSize = data.size();
    int tagOffset = headerSize + header[1] + header[2];
    if(tagOffset>=fileSize)
        return false;

    pBuf += tagOffset;

    if(strncmp(pBuf, "[TAG]", 5) != 0)
        return false;

    QString s = pBuf+5;
    fillTrackInfo(s, info);
    return true;
}

TFileParse::TFileParse(QString file) :
    TAbstractParse(file)
{

}

bool TFileParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    wstring wpath = mFile.toStdWString();
    TTrackInfo *trackInfo = new TTrackInfo;
    if(!getMusicInfo(wpath.c_str(), trackInfo))
        return false;

    musicInfo->musicName = trackInfo->trackName;
    musicInfo->duration = trackInfo->duration;
    musicInfo->additionalInfo = trackInfo->additionalInfo;
    musicInfo->trackList.push_back(trackInfo);

    return true;
}

TTrackInfo *TFileParse::parse(QByteArray data)
{
    TTrackInfo *trackInfo = new TTrackInfo;
    if(!getMusicInfo(data, trackInfo))
    {
        delete trackInfo;
        return NULL;
    }
    return trackInfo;
}
