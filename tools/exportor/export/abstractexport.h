#ifndef TABSTRACTEXPORT_H
#define TABSTRACTEXPORT_H

#include <stdio.h>
#include <string.h>

class TAbstractExport
{
public:
    TAbstractExport(wchar_t *fileName);
    ~TAbstractExport();

    virtual void start() = 0;
    virtual void end() = 0;
    virtual int write(char *buf, int size) = 0;

protected:
    void open();
    void close();
    int seek(long offset, int orgin);
    long tell();
    int writeInt(int data);
    int writeShort(short data);
    int writeDouble(double data);
    int writeString(const char *pStr);

private:
    wchar_t mFileName[260];
    FILE *mFile;
};

#endif // TABSTRACTEXPORT_H
