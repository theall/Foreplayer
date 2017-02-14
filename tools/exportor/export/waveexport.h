#ifndef TWAVEEXPORT_H
#define TWAVEEXPORT_H

#include "abstractexport.h"

class TWaveExport : public TAbstractExport
{
public:
    TWaveExport(const wchar_t *fileName, int sampleRate);
    ~TWaveExport();

    int write(const byte *buf, int size);

private:
    int mSampleRate;

    void start();
    void end();
};

#endif // TWAVEEXPORT_H
