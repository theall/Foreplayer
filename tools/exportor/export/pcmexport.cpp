#include "pcmexport.h"


TPcmExport::TPcmExport(const wchar_t *fileName, int sampleRate) :
    TAbstractExport(fileName)
{
    (void)sampleRate;

    start();
}

TPcmExport::~TPcmExport()
{
    end();
}

void TPcmExport::start()
{

}

void TPcmExport::end()
{
}

int TPcmExport::write(const byte *buf, int size)
{
    short *data = (short *)buf;
    for (int i = 0; i < size/2; i++)
    {
        writeShort(Endian16_Swap(data[i]));
    }
    return size;
}

