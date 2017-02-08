#include "zipparse.h"

#define ZIP_FILE_NAME_SIZE 256
#define ZIP_FILE_BUF_SIZE 16384
#define ZIP_FILE_MAX_SIZE 4194304

TZipParse::TZipParse(QString file) :
    TAbstractParse(file)
{

}

TZipParse::~TZipParse()
{

}

bool TZipParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    std::string fileName = mFile.toStdString();
    unzFile zipFile = unzOpen(fileName.c_str());
    if(!zipFile)
        return false;

    int result = unzGoToFirstFile(zipFile);
    if(result != UNZ_OK)
    {
        unzClose(zipFile);
        return false;
    }
    while (true)
    {
        unz_file_info fileInfo;
        memset(&fileInfo, 0, sizeof(unz_file_info));

        char szFileName[ZIP_FILE_NAME_SIZE];
        unzGetCurrentFileInfo(zipFile, &fileInfo, szFileName, ZIP_FILE_NAME_SIZE, NULL, 0, NULL, 0);

        // Only process file size < ZIP_FILE_MAX_SIZE
        if(fileInfo.uncompressed_size < ZIP_FILE_MAX_SIZE)
        {
            result = unzOpenCurrentFile(zipFile);
            if(result == UNZ_OK)
            {
                QByteArray dataBuf("");
                int sizeNeed = fileInfo.uncompressed_size;
                dataBuf.resize(sizeNeed);
                if(unzReadCurrentFile(zipFile, dataBuf.data(), sizeNeed) == sizeNeed)
                {
                    TFileParse fileParse;
                    TTrackInfo *track = fileParse.parse(dataBuf);
                    if(track)
                    {
                        //__int64 UnpSize = HeaderData.UnpSize+(((__int64)HeaderData.UnpSizeHigh)<<32);
                        //__int64 PackSize = HeaderData.PackSize+(((__int64)HeaderData.PackSizeHigh)<<32);
                        track->fileSize = sizeNeed;
                        track->indexName = szFileName;
                        musicInfo->duration += track->duration;
                        musicInfo->trackList.push_back(track);
                    }
                }
                unzCloseCurrentFile(zipFile);
            }
        }
        result = unzGoToNextFile(zipFile);
        if(result != UNZ_OK)
            break;
    }

    unzClose(zipFile);

    if(musicInfo->trackList.size() > 0)
    {
        musicInfo->additionalInfo = musicInfo->trackList[0]->additionalInfo;
        return true;
    }

    return false;
}

QByteArray TZipParse::trackData(TTrackInfo *trackInfo)
{
    if(!trackInfo)
        return QByteArray();

    QByteArray dataBuf("");

    unzFile zipFile = unzOpen(trackInfo->musicFileName.c_str());
    if(!zipFile)
        return dataBuf;

    int result = unzLocateFile(zipFile, trackInfo->indexName.c_str(), false);
    if(result != UNZ_OK)
    {
        unzClose(zipFile);
        return dataBuf;
    }

    result = unzOpenCurrentFile(zipFile);
    if(result == UNZ_OK)
    {
        char buf[ZIP_FILE_BUF_SIZE];
        int readLength = 0;
        while(true)
        {
            readLength = unzReadCurrentFile(zipFile, buf, ZIP_FILE_BUF_SIZE);
            if(readLength==UNZ_ERRNO)
            {
                break;
            } else if (readLength==0) {
                break;
            } else if (readLength>0) {
                dataBuf.append(buf, readLength);
            }
        }
    }
    unzCloseCurrentFile(zipFile);
    unzClose(zipFile);
    return dataBuf;
}
