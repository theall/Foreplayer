#include "abstractexport.h"

TAbstractExport::TAbstractExport(const wchar_t *fileName) :
    mFile(NULL)
{
    if(fileName)
    {
        wcscpy(mFileName, fileName);
        open();
    }
}

TAbstractExport::~TAbstractExport()
{
    close();
}

bool TAbstractExport::isOpened()
{
    return mFile!=NULL;
}

void TAbstractExport::open()
{
    close();
    mFile = _wfopen(mFileName, L"wb");
}

void TAbstractExport::close()
{
    if(mFile)
    {
        fclose(mFile);
        mFile = NULL;
    }
}

int TAbstractExport::seek(long offset, int orgin)
{
    if(mFile)
        return fseek(mFile, offset, orgin);

    return -1;
}

long TAbstractExport::tell()
{
    if(mFile)
        return ftell(mFile);

    return -1;
}

int TAbstractExport::writeByte(const byte *buf, int size)
{
    if(mFile)
        return fwrite(buf, 1, size, mFile);
    return 0;
}

int TAbstractExport::writeInt(int data)
{
    if(mFile)
        return fwrite(&data, sizeof(int), 1, mFile);
    return 0;
}

int TAbstractExport::writeShort(short data)
{
    if(mFile)
        return fwrite(&data, sizeof(short), 1, mFile);
    return 0;
}

int TAbstractExport::writeDouble(double data)
{
    if(mFile)
        return fwrite(&data, sizeof(double), 1, mFile);
    return 0;
}

int TAbstractExport::writeString(const char *pStr)
{
    if(mFile)
        return fwrite(pStr, 1, strlen(pStr), mFile);

    return 0;
}

void TAbstractExport::setTitle(std::string title)
{
    mTitle = title;
}

void TAbstractExport::setArtist(std::string artist)
{
    mArtist = artist;
}

void TAbstractExport::setAlbum(std::string album)
{
    mAlbum = album;
}

void TAbstractExport::setYear(std::string year)
{
    mYear = year;
}

void TAbstractExport::setYear(int year)
{
    char buf[10];
    *buf = 0;
    mYear = itoa(year, buf, 10);
}

void TAbstractExport::setComment(std::string comment)
{
    mComment = comment;
}
