#ifndef TWAVEEXPORT_H
#define TWAVEEXPORT_H

#include "abstractexport.h"

class TWaveExport : public TAbstractExport
{
public:
    TWaveExport(const wchar_t *fileName, int sampleRate);
    ~TWaveExport();

    int write(const byte *buf, int size);

    void setTitle(std::string title);
    void setArtist(std::string artist);
    void setAlbum(std::string album);
    void setYear(std::string year);
    void setYear(int year);
    void setComment(std::string comment);

private:
    int mSampleRate;

    void start();
    void end();
};

#endif // TWAVEEXPORT_H
