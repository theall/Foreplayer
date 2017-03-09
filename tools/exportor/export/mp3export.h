#ifndef TMP3EXPORT_H
#define TMP3EXPORT_H

#include "abstractexport.h"
#include <lame.h>

class TMP3Export : public TAbstractExport
{
public:
    TMP3Export(const wchar_t *fileName, int sampleRate=44100);
    ~TMP3Export();

    void setChannels(int channels);
    void setQuality(int quality);
    void setVbr(vbr_mode_e vbr);
    void setBrate(int brate);

    int write(const byte *buf, int size);

private:
    lame_t mLame;
    vbr_mode mVbr;
    int mBrate;
    int mQuality;
    int mChannels;
    int mSampleRate;

    void start();
    void end();
};

#endif // TMP3EXPORT_H
