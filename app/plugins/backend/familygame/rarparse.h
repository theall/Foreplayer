#ifndef TRARPACKAGE_H
#define TRARPACKAGE_H

#include <unrar/unrar.h>
#include "fileparse.h"

class TRarParse : public TAbstractParse
{
public:
    explicit TRarParse(QString file);
    ~TRarParse();

    bool parse(TMusicInfo *musicInfo) Q_DECL_OVERRIDE;
    QByteArray trackData(TTrackInfo *trackInfo);

private:
};

#endif // TRARPACKAGE_H
