#include <QCoreApplication>
#include <QSharedMemory>

#include "../../app/core/core.h"
#include "export/exportfactory.h"
#include "commandline/cmdlineparser.h"

#define trp(x) qPrintable(QCoreApplication::translate("main", x))

typedef void (*IProgressCallback)(int value, int max);

struct TrackItemParam
{
    QString sourceFile;
    int sampleRate = 0;
    int duration = 0;
};

bool isBlankFrame(byte *buf, int size)
{
    short *pBuf = (short*)buf;
    int validPcm = 0;
    int sumFrame = 0;
    for(int i=0;i<size/2;i++)
    {
        short amp = abs(*pBuf);
        if(amp > 16)
            validPcm++;

        sumFrame += amp;
        pBuf++;
    }
    return sumFrame==0 || (float)validPcm/size<0.05;
}

bool exportTrack(
        TCore *core,
        TrackItem trackItem,
        TrackItemParam *param,
        QString outputFile,
        QString format,
        TExportParam *exportParam = NULL
        )
{
    if(!core || !trackItem)
        return false;

    if(!core->loadTrackItem(trackItem))
    {
        printf(trp("Failed to load track %s in %s\n"), qPrintable(core->getTrackItemIndexName(trackItem)), qPrintable(param->sourceFile));
        return false;
    }
    TExportFactory exportFactory;
    TAbstractExport *exportor = NULL;
    wstring fileNameW = outputFile.toStdWString();
    if(format.isEmpty())
        exportor = exportFactory.create(fileNameW.c_str(), param->sampleRate);
    else
        exportor = exportFactory.create(format, fileNameW.c_str(), param->sampleRate);

    if(!exportor->isOpened())
    {
        printf(trp("Failed to create exporter of %s\n"), qPrintable(outputFile));
        return false;
    }
    if(TMP3Export *mp3Exportor=reinterpret_cast<TMP3Export*>(exportor))
    {
        mp3Exportor->setTitle(core->getTrackItemName(trackItem).toStdString());
        mp3Exportor->setArtist(core->getTrackItemArtist(trackItem).toStdString());
        mp3Exportor->setAlbum(core->getTrackItemAlbum(trackItem).toStdString());
        mp3Exportor->setYear(core->getTrackItemYear(trackItem).toInt());
        mp3Exportor->setComment(core->getTrackItemAdditionalInfo(trackItem).toStdString());
    }
    int samples = core->getFrameSampleCount(param->sampleRate, 600);
    if(samples < 2)
        samples = 4096;

    int bufSize = samples*4;
    byte *buf = (byte*)malloc(bufSize);
    int silentFrames = 0;
    float framesPerSec = param->sampleRate/samples;

    int realDuration = core->getTrackItemDuration(trackItem);
    if(realDuration < 1)
        realDuration = 150000;

    int totalFrames = framesPerSec*realDuration/1000 + 0.5;
    if(exportParam)
        exportParam->progressTotalFrames = totalFrames;

    for(int i=0;i<totalFrames;i++)
    {
        if(exportParam)
        {
            while(exportParam->state!=ES_RUN)
                QThread::msleep(500);
        }
        memset(buf, 0, bufSize);
        core->getNextFrame((char*)buf, bufSize);
        if(isBlankFrame(buf, bufSize))
        {
            silentFrames++;
        } else {
            silentFrames = 0;
        }
        if(silentFrames >= framesPerSec*3){
            // Frames end
            break;
        }
        exportor->write((const byte*)buf, bufSize);
        if(exportParam)
            exportParam->progressCurrentFrames = i+1;
    }
    if(exportParam)
        exportParam->state = ES_COMPLETE;
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("Exportor");
    QCoreApplication::setApplicationVersion("1.0.0");

    TCmdlineParser parser(a.arguments());
    if(parser.needHelp())
    {
        parser.showHelp();
        return 0;
    }
    if(parser.isError())
    {
        printf(trp("Music file path is invalid or non-exists.\n"));
        return 0;
    }
    // Load backend plugins
    TCore core;
    QString indexName = parser.indexName();
    QString sourceFile = parser.sourceFile();
    QString destFilePath = parser.destFilePath();
    QString format = parser.format();

    int sampleRate = parser.sampleRate();
    int duration = parser.duration();
    bool overWriteFile = parser.overWrite();
    bool bVerbose = parser.verbose();
    bool isListMode = parser.isListMode();

    TExportParam *exportParam = NULL;
    if(parser.runAsDaemon())
        exportParam = TCmdlineParser::getExportParam();

    // Parse track list
    MusicItem musicItem = core.parse(sourceFile);
    if(!musicItem)
    {
        printf(trp("Music file can not be parsed.\n"));
        return 0;
    }

    int trackCount = core.getTrackItemCount(musicItem);
    if(trackCount == 0)
    {
        printf(trp("No tracks in music file.\n"));
        return 0;
    } else if(bVerbose) {
        printf(trp("Find %d tracks in music file.\n"), trackCount);
    }

    TrackItems trackItems;
    if(!indexName.isEmpty())
    {
        TrackItem trackTarget = NULL;
        for(int i=0;i<trackCount;i++)
        {
            TrackItem trackItem = core.getTrackItem(musicItem, i);
            if(core.getTrackItemIndexName(trackItem).toLower() == indexName.toLower())
            {
                trackTarget = trackItem;
                break;
            }
        }
        if(trackTarget)
            trackItems.push_back(trackTarget);
        else
        {
            printf(trp("Failed to find specified track with index %s\n"), qPrintable(indexName));
            return 0;
        }
    } else {
        trackItems = core.getTrackItems(musicItem);
    }

    bool destIsDir = true;
    QFileInfo fi(sourceFile);

    if(destFilePath.isEmpty())
    {
        destFilePath = fi.absolutePath();
    } else {
        QFileInfo dfi(destFilePath);
        if(dfi.isRelative())
            destFilePath = fi.absoluteDir().filePath(destFilePath);

        destIsDir = dfi.isDir();
        if(dfi.suffix().isEmpty() && !dfi.exists())
            dfi.absoluteDir().mkpath(destFilePath);
    }

    QDir destDir(destFilePath);
    int trackListSize = trackItems.size();

    int nSkip = 0;
    int nSuccess = 0;
    int nFail = 0;
    for(int i=0;i<trackListSize;i++)
    {
        TrackItem trackItem = trackItems[i];
        QString trackItemName = core.getTrackItemName(trackItem);
        if(isListMode) {
            printf("%s %s\n", qPrintable(core.getTrackItemIndexName(trackItem)), qPrintable(trackItemName));
        } else {
            TrackItemParam param;
            param.sampleRate = sampleRate;
            if(core.getTrackItemDuration(trackItem) <= 0)
                param.duration = duration;

            QString destFileFullName;
            if((trackListSize>1 && indexName.isEmpty()) || destIsDir)
            {
                // While exporting muliple tracks, auto set dest name to track name
                QString baseName = trackItemName;
                baseName.prepend(QString("%1.").arg(i+1));
                if(QFileInfo(baseName).suffix().toLower() != format.toLower())
                    baseName += "."+format;
                destFileFullName = destDir.absoluteFilePath(baseName);
            } else {
                destFileFullName = destFilePath;
            }
            if(QFileInfo(destFileFullName).exists() && !overWriteFile)
            {
                if(bVerbose)
                    printf(trp("Warning, destination file exists, %s\n"), qPrintable(destFileFullName));

                nSkip++;
                continue;
            }
            if(exportTrack(&core, trackItem, &param, destFileFullName, format, exportParam))
            {
                nSuccess++;
                printf(trp("Export to %s\n"), qPrintable(destFileFullName));
            } else {
                nFail++;
            }
        }

    }
    if(!isListMode)
    {
        if(trackListSize <= 0)
        {
            printf(trp("No tracks exported.\n"));
        } else if(trackListSize > 1) {
            printf(trp("Success: %d, Failed: %d, Skipped: %d.\n"), nSuccess, nFail, nSkip);
        }
    }

    a.quit();
    return 0;
}

