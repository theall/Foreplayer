#include "rarparse.h"

#include <QDebug>
#include <QByteArray>

#define RAR_FILE_MAX_SIZE 4194304

enum ERR_TYPE {
    ERR_OPEN,
    ERR_READ,
    ERR_PROCESS
};
enum USER_DATA {
    EXTRACT,
    TEST,
    PRINT,
    LIST
};

struct TUserData
{
    QByteArray buffer;
    int sizeNeed;
};

#define BUF_SIZE 16384

void OutError(int Error,char *ArcName,int ErrType)
{
    switch(Error)
    {
        case ERAR_NO_MEMORY:
            printf("\nNot enough memory");
            break;
        case ERAR_BAD_DATA:
            printf("\n%s: archive header or data are damaged", ArcName);
            break;
        case ERAR_BAD_ARCHIVE:
            printf("\n%s is not RAR archive", ArcName);
            break;
        case ERAR_UNKNOWN_FORMAT:
            printf("Unknown archive format");
            break;
        case ERAR_EOPEN:
            if (ErrType==ERR_PROCESS) // Returned by RARProcessFile.
                printf("Volume open error");
            else
                printf("\nCannot open %s",ArcName);
            break;
        case ERAR_ECREATE:
            printf("File create error");
            break;
        case ERAR_ECLOSE:
            printf("File close error");
            break;
        case ERAR_EREAD:
            printf("Read error");
            break;
        case ERAR_EWRITE:
            printf("Write error");
            break;
        case ERAR_SMALL_BUF:
            printf("Buffer for archive comment is too small, comment truncated");
            break;
        case ERAR_UNKNOWN:
            printf("Unknown error");
            break;
        case ERAR_MISSING_PASSWORD:
            printf("Password for encrypted file or header is not specified");
            break;
        case ERAR_EREFERENCE:
            printf("Cannot open file source for reference record");
            break;
        case ERAR_BAD_PASSWORD:
            printf("Wrong password is specified");
            break;
    }
}

int CALLBACK CallbackProc(UINT msg, LPARAM userdata, LPARAM p1, LPARAM p2)
{
    if(msg == UCM_PROCESSDATA)
    {
        TUserData *ud = (TUserData*)userdata;
        if(ud)
        {
            if(ud->buffer.size() < ud->sizeNeed)
                ud->buffer.append((char *)p1, p2);
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

TRarParse::TRarParse(QString file) :
    TAbstractParse(file)
{

}

TRarParse::~TRarParse()
{

}

bool TRarParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    HANDLE hArcData;
    int RHCode;
    char *cmtBuf = new char[BUF_SIZE];
    TUserData userData;

    RAROpenArchiveDataEx openArchiveData;
    wchar_t RedirName[1024];
    std::string arcNameStr = mFile.toStdString();
    const char *arcName = (char*)arcNameStr.c_str();

    RARHeaderDataEx HeaderData;
    memset(&HeaderData, 0, sizeof(HeaderData));
    memset(&openArchiveData, 0, sizeof(openArchiveData));

    openArchiveData.ArcName     = (char*)arcName;
    openArchiveData.CmtBuf      = cmtBuf;
    openArchiveData.CmtBufSize  = BUF_SIZE;
    openArchiveData.OpenMode    = RAR_OM_EXTRACT;
    openArchiveData.Callback    = CallbackProc;
    openArchiveData.UserData    = (LPARAM)&userData;
    hArcData = RAROpenArchiveEx(&openArchiveData);

    if (openArchiveData.OpenResult != 0)
    {
        qDebug() << "fail to open file, " << openArchiveData.OpenResult;
        return false;
    }
    HeaderData.RedirName = RedirName;
    HeaderData.RedirNameSize = sizeof(RedirName) / sizeof(RedirName[0]);

    while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
    {
        userData.buffer.clear();
        userData.sizeNeed = (int)HeaderData.UnpSize;
        int error = RARProcessFile(hArcData, RAR_EXTRACT, NULL, NULL);
        if(error == ERAR_SUCCESS && userData.sizeNeed == userData.buffer.size())
        {
            QString currentFile = HeaderData.FileName;
            currentFile = currentFile.toLower();
            if(currentFile=="info.txt")
                musicInfo->additionalInfo = userData.buffer.constData();
            else {
                TFileParse fileParse;
                TTrackInfo *track = fileParse.parse(userData.buffer);
                if(track)
                {
                    //__int64 UnpSize = HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
                    //__int64 PackSize = HeaderData.PackSize+(((__int64)HeaderData.PackSizeHigh)<<32);
                    track->fileSize = HeaderData.UnpSize;
                    track->indexName = currentFile.toStdString();
                    musicInfo->duration += track->duration;
                    musicInfo->trackList.push_back(track);
                }
            }
        } else {
            qDebug() << error;
        }
        // qDebug() << HeaderData.FileName << userData.sizeNeed << userData.buffer.size();
    }
    RARCloseArchive(hArcData);

    delete cmtBuf;

    if(musicInfo->trackList.size() > 0)
    {
        if(musicInfo->additionalInfo.empty())
            musicInfo->additionalInfo = musicInfo->trackList[0]->additionalInfo;

        return true;
    }

    return false;
}

QByteArray TRarParse::trackData(TTrackInfo *trackInfo)
{
    if(!trackInfo)
        return QByteArray();

    HANDLE hArcData;
    int RHCode;
    char *cmtBuf = new char[BUF_SIZE];
    TUserData userData;

    RAROpenArchiveDataEx openArchiveData;
    wchar_t RedirName[1024];
    std::string arcNameStr = mFile.toStdString();
    const char *arcName = (char*)arcNameStr.c_str();

    RARHeaderDataEx HeaderData;
    memset(&HeaderData, 0, sizeof(HeaderData));
    memset(&openArchiveData, 0, sizeof(openArchiveData));

    openArchiveData.ArcName     = (char*)arcName;
    openArchiveData.CmtBuf      = cmtBuf;
    openArchiveData.CmtBufSize  = BUF_SIZE;
    openArchiveData.OpenMode    = RAR_OM_EXTRACT;
    openArchiveData.Callback    = CallbackProc;
    openArchiveData.UserData    = (LPARAM)&userData;
    hArcData = RAROpenArchiveEx(&openArchiveData);

    if (openArchiveData.OpenResult != 0)
    {
        qDebug() << "fail to open file, " << openArchiveData.OpenResult;
        return userData.buffer;
    }
    HeaderData.RedirName = RedirName;
    HeaderData.RedirNameSize = sizeof(RedirName) / sizeof(RedirName[0]);

    while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
    {
        if(trackInfo->indexName==HeaderData.FileName && HeaderData.UnpSize<RAR_FILE_MAX_SIZE)
        {
            userData.buffer.clear();
            userData.sizeNeed = (int)HeaderData.UnpSize;
            int error = RARProcessFile(hArcData, RAR_EXTRACT, NULL, NULL);
            if(error == ERAR_SUCCESS && userData.sizeNeed == userData.buffer.size())
            {

            } else {
                qDebug() << error;
            }
            break;
        } else {
            RARProcessFile(hArcData, RAR_SKIP, NULL, NULL);
        }
    }
    RARCloseArchive(hArcData);

    delete cmtBuf;
    return userData.buffer;
}
