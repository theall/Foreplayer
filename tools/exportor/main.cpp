#include <QCoreApplication>
#include <QSharedMemory>
#include <QCommandLineOption>
#include <QCommandLineParser>

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
#define tr(x) QCoreApplication::translate("main", x)

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Exportor");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.addOption({{"i", "index"}, tr("Index name of track."), "index", "0"});
    parser.addOption({{"d", "duration"}, tr("Specify the duration of track.The default duration will be used if this parameter is invalid."), "seconds"});
    parser.addOption({{"r", "sample rate"}, tr("Specify the sample rate, default is 44100HZ."), "sample_rate", "44100"});

    parser.addPositionalArgument("source", tr("The music file full name to export."));
    parser.addPositionalArgument("destination", tr("The destination file path to export."), "[filename]");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("Export track to wave/mp3 from music file.");
    parser.process(a);
    return 0;
    TBackendPluginManager *plugins = TBackendPluginManager::instance();
    TTrackInfo trackInfo;
    trackInfo.musicFileName = "z:/spc/mmax2.rsn";
    trackInfo.indexName = "mmax2-03.spc";
    trackInfo.duration = 110000;
//    trackInfo.musicFileName = "z:/spc/dino.zip";
//    trackInfo.index = 0<<16 | 1;
//    trackInfo.duration = 36000;

    if(trackInfo.duration < 1)
        trackInfo.duration = 0x7fffffff;

    TBackendPlugin *plugin = plugins->loadTrack(&trackInfo);
    if(plugin)
    {
        TRequestSamples nextframe = plugin->getCallback();
        TWaveExport waveExportor((wchar_t*)L"z:/test.wav", SAMPLE_RATE);
        int samples = plugin->getSampleSize(SAMPLE_RATE, SOUND_FPS);
        if(samples < 2)
            samples = 4096;
        int bufSize = samples*4;
        short *buf = (short*)malloc(bufSize);
        int silentFrames = 0;
        float framesPerSec = SAMPLE_RATE*4/bufSize;

        int totalFrames = framesPerSec*((float)trackInfo.duration/1000+0.5) + 0.5;

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
            }
            waveExportor.write((char*)buf, bufSize);
        }
    }
    a.quit();
    return 0;
}

