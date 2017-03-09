#ifndef TPCMEXPORT_H
#define TPCMEXPORT_H

#include "abstractexport.h"

class TPcmExport : public TAbstractExport
{
public:
    TPcmExport(const wchar_t *fileName, int sampleRate = 0);
    ~TPcmExport();

    int write(const byte *buf, int size);

    void setTitle(std::string title);
    void setArtist(std::string artist);
    void setAlbum(std::string album);
    void setYear(std::string year);
    void setYear(int year);
    void setComment(std::string comment);

private:
    void start();
    void end();
};

#endif // TPCMEXPORT_H
