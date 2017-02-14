#ifndef TPCMEXPORT_H
#define TPCMEXPORT_H

#include "abstractexport.h"

class TPcmExport : public TAbstractExport
{
public:
    TPcmExport(const wchar_t *fileName, int sampleRate = 0);
    ~TPcmExport();

    int write(const byte *buf, int size);

private:
    void start();
    void end();
};

#endif // TPCMEXPORT_H
