#ifndef TMP3EXPORT_H
#define TMP3EXPORT_H

#include "abstractexport.h"
#include <lame.h>

#define TITLE_LENGTH    256
class TMP3Export : public TAbstractExport
{
public:
    TMP3Export(const wchar_t *fileName, int sampleRate=44100);
    ~TMP3Export();

    void setChannels(int channels);
    void setQuality(int quality);
    void setVbr(vbr_mode_e vbr);
    void setBrate(int brate);
    void setTitle(const char *title);
    void setArtist(const char *artist);
    void setAlbum(const char *album);

    int write(const byte *buf, int size);

private:
    lame_t mLame;
    vbr_mode mVbr;
    int mBrate;
    int mQuality;
    int mChannels;
    int mSampleRate;

    char mTitle[TITLE_LENGTH];
    char mArtist[TITLE_LENGTH];
    char mAlbum[TITLE_LENGTH];

    void start();
    void end();
};

#endif // TMP3EXPORT_H
