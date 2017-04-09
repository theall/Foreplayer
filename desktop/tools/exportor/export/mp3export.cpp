#include "mp3export.h"

#define MP3BUFSIZE(x) (int)(1.25*x)+7200

TMP3Export::TMP3Export(const wchar_t *fileName, int sampleRate) :
    TAbstractExport(fileName)
  , mLame(NULL)
  , mVbr(vbr_default)
  , mBrate(2)
  , mQuality(0)
  , mChannels(2)
  , mSampleRate(sampleRate)
{

}

TMP3Export::~TMP3Export()
{
    end();
}

void TMP3Export::setChannels(int channels)
{
    mChannels = channels;
}

void TMP3Export::setQuality(int quality)
{
    mQuality = quality;
}

void TMP3Export::setVbr(vbr_mode_e vbr)
{
    mVbr = vbr;
}

void TMP3Export::setBrate(int brate)
{
    mBrate = brate;
}

void TMP3Export::start()
{
    if(!mLame)
    {
        mLame = lame_init();
        lame_set_num_channels(mLame, mChannels);
        lame_set_in_samplerate(mLame, mSampleRate);
        id3tag_set_title(mLame, mTitle.c_str());
        id3tag_set_artist(mLame, mArtist.c_str());
        id3tag_set_album(mLame, mAlbum.c_str());
        id3tag_set_year(mLame, mYear.c_str());
        id3tag_set_comment(mLame, mComment.c_str());
        lame_init_params(mLame);
    }
}

void TMP3Export::end()
{
    if(mLame)
    {
        int mp3BufSize = 7200 + 8192;
        unsigned char mp3Buf[mp3BufSize];
        int wroteSize = lame_encode_flush(mLame, mp3Buf, mp3BufSize);
        if(wroteSize > 0)
            writeByte(mp3Buf, wroteSize);
        lame_close(mLame);
        mLame = NULL;
    }
}

int TMP3Export::write(const byte *buf, int size)
{
    if(!mLame)
        start();

    if(!mLame)
        return -1;

    int samples = size / 4;
    int mp3BufSize = MP3BUFSIZE(samples*2);
    unsigned char mp3Buf[mp3BufSize];
    memset(mp3Buf, 0, mp3BufSize);
    int wroteSize = lame_encode_buffer_interleaved(mLame, (short*)buf, samples, mp3Buf, mp3BufSize);
    return writeByte(mp3Buf, wroteSize);
}
