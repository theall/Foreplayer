#include "rarparse.h"

#include <QDebug>

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

#define BUF_SIZE 262144
RARHeaderDataEx HeaderData;

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
        TMusicInfo *musicInfo = (TMusicInfo*)userdata;
        qDebug() << HeaderData.FileName << p2;
        if(musicInfo)
        {
            QByteArray data((char *)p1, p2);
            if(data.size() == (int)HeaderData.UnpSize)
            {
                QString currentFile = HeaderData.FileName;
                currentFile = currentFile.toLower();
                if(currentFile=="info.txt")
                    musicInfo->additionalInfo = data.constData();
                else {
                    TFileParse fileParse;
                    TTrackInfo *track = fileParse.parse(data);
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
                return -1;
            }
        }
        return 1;
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
    char cmtBuf[BUF_SIZE];
    RAROpenArchiveDataEx openArchiveData;
    wchar_t RedirName[1024];
    const char *arcName = (char*)mFile.toStdString().c_str();

    memset(&HeaderData, 0, sizeof(HeaderData));
    memset(&openArchiveData, 0, sizeof(openArchiveData));

    openArchiveData.ArcName     = (char*)arcName;
    openArchiveData.CmtBuf      = cmtBuf;
    openArchiveData.CmtBufSize  = BUF_SIZE;
    openArchiveData.OpenMode    = RAR_OM_EXTRACT;
    openArchiveData.Callback    = CallbackProc;
    openArchiveData.UserData    = (LPARAM)musicInfo;
    hArcData = RAROpenArchiveEx(&openArchiveData);

    if (openArchiveData.OpenResult != 0)
    {
        qDebug() << "Fait to open file, " << openArchiveData.OpenResult;
        return false;
    }

    // ShowArcInfo(openArchiveData.Flags, arcName);

    HeaderData.RedirName = RedirName;
    HeaderData.RedirNameSize = sizeof(RedirName) / sizeof(RedirName[0]);

    while ((RHCode=RARReadHeaderEx(hArcData,&HeaderData))==0)
    {
        RARProcessFile(hArcData, RAR_EXTRACT, NULL, NULL);
    }
    RARCloseArchive(hArcData);

    return true;
}
