#ifndef TABSTRACTEXPORT_H
#define TABSTRACTEXPORT_H

#include <stdio.h>
#include <string.h>

#if 1
#define Endian16_Swap(x) (x)
#define Endian_16_Swap(x) (x)
#define Endian32_Swap(x) (x)
#else
#define Endian16_Swap(x) (((x)<<8) | ((x)>>8))
#define Endian_16_Swap(x) (((x)<<8) | ((x)>>8))
#define Endian32_Swap(x) ( ((x&0xff000000)>>24) || ((x&0x00ff0000)>>8) || ((x&0x0000ff00)<<8) || ((x&0x000000ff)<<24) )
#endif

typedef unsigned char byte;

class TAbstractExport
{
public:
    TAbstractExport(const wchar_t *fileName);
    ~TAbstractExport();

    virtual void start() = 0;
    virtual void end() = 0;
    virtual int write(const byte *buf, int size) = 0;

    bool isOpened();

protected:
    void open();
    void close();
    int seek(long offset, int orgin);
    long tell();
    int writeByte(const byte *buf, int size);
    int writeInt(int data);
    int writeShort(short data);
    int writeDouble(double data);
    int writeString(const char *pStr);

private:
    wchar_t mFileName[260];
    FILE *mFile;
};

#endif // TABSTRACTEXPORT_H
