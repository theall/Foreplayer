#include "fileparse.h"
#include <QString>

TFileParse::TFileParse(QString file) :
    TAbstractParse(file)
{

}

bool TFileParse::parse(TMusicInfo *musicInfo)
{
    if(!musicInfo)
        return false;

    TGmeWrap *gme = TGmeWrap::instance();
    gme->loadFile(mFile.toLocal8Bit().constData());
    int trackCount = gme->trackCount();
    int trackIndex = 1;
    for(int i=0;i<trackCount;i++) {
        TTrackInfo *trackInfo = new TTrackInfo;
        const gme_info_t *_trackInfo = gme->trackInfo(i);
        trackInfo->trackName = _trackInfo->song;
        trackInfo->additionalInfo = fillAdditionalInfo(_trackInfo);
        trackInfo->duration  = _trackInfo->length/1000;
        trackInfo->index     = trackIndex;
        musicInfo->trackList.push_back(trackInfo);
        musicInfo->duration += trackInfo->duration;
        trackIndex++;
    }
    if(trackCount > 0)
    {
        TTrackInfo *trackInfo = musicInfo->trackList.at(0);
        musicInfo->additionalInfo = trackInfo->additionalInfo;
        if(trackCount==1)
            musicInfo->musicName    = trackInfo->trackName;
    }

    return true;
}

TTrackInfo *TFileParse::parse(QByteArray data)
{
    TGmeWrap *gme = TGmeWrap::instance();
    gme->loadData((void*)data.constData(), data.size());
    int trackCount = gme->trackCount();
    if(trackCount != 1)
        return NULL;

    TTrackInfo *trackInfo = new TTrackInfo;
    const gme_info_t *_trackInfo = gme->trackInfo(0);
    trackInfo->trackName = _trackInfo->song;
    trackInfo->index = 1;
    trackInfo->duration  = _trackInfo->length/1000;
    trackInfo->fileSize  = data.size();
    trackInfo->additionalInfo = fillAdditionalInfo(_trackInfo);

    return trackInfo;
}

string TFileParse::fillAdditionalInfo(const gme_info_t *trackInfo)
{
    return QString::asprintf("game:%s\n"
                    "system:%s\n"
                    "author:%s\n"
                    "dumper:%s\n"
                    "copyright:%s\n"
                    "comment:%s\n",
                    trackInfo->game,
                    trackInfo->system,
                    trackInfo->author,
                    trackInfo->dumper,
                    trackInfo->copyright,
                    trackInfo->comment
                    ).toStdString();
}
