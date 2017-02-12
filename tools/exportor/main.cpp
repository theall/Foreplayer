#include <QCoreApplication>
#include <QSharedMemory>

#include "../../app/core/core.h"
#include "export/waveexport.h"

bool isBlankFrame(short *buf, int size)
{
    short *pBuf = buf;
    int validPcm = 0;
    int sumFrame = 0;
    for(int i=0;i<size;i++)
    {
        short amp = abs(*pBuf);
        if(amp > 16)
            validPcm++;

        sumFrame += amp;
        pBuf++;
    }
    return sumFrame==0 || (float)validPcm/size<0.05;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TBackendPluginManager *plugins = TBackendPluginManager::instance();
    TTrackInfo trackInfo;
    trackInfo.musicFileName = "z:/spc/mmax2.rsn";
    trackInfo.indexName = "mmax2-03.spc";
    trackInfo.duration = 110000;
//    trackInfo.musicFileName = "z:/spc/dino.zip";
//    trackInfo.index = 0<<16 | 1;
//    trackInfo.duration = 36000;

    if(trackInfo.duration < 1)
        trackInfo.duration = 60000;

    TBackendPlugin *plugin = plugins->loadTrack(&trackInfo);
    if(plugin)
    {
        TRequestSamples nextframe = plugin->getCallback();
        TWaveExport waveExportor((wchar_t*)L"z:/test.wav", SAMPLE_RATE);
        int samples = plugin->getSampleSize(SAMPLE_RATE, SOUND_FPS);
        if(samples < 2)
            samples = 4096;
        int bufSize = samples*2;
        short *buf = (short*)malloc(bufSize);
        int silentFrames = 0;
        float framesPerSec = (float)SAMPLE_RATE*4/bufSize;

        int totalFrames = framesPerSec * trackInfo.duration/1000 + 0.5;
        for(int i=0;i<totalFrames;i++)
        {
            memset(buf, 0, bufSize);
            nextframe(samples, buf);
            if(isBlankFrame(buf, samples))
            {
                silentFrames++;
            } else {
                silentFrames = 0;
            }
            if(silentFrames >= framesPerSec){
                // Frames end
                break;
            } else {
                waveExportor.write((char*)buf, bufSize);
            }
        }
    }
    a.quit();
    return 0;
}

