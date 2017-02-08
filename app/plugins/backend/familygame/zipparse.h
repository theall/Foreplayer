#ifndef TZIPPARSE_H
#define TZIPPARSE_H

#include <unzip/unzip.h>

#include "fileparse.h"

class TZipParse : public TAbstractParse
{
public:
    explicit TZipParse(QString file);
    ~TZipParse();

    bool parse(TMusicInfo *musicInfo) Q_DECL_OVERRIDE;
    QByteArray trackData(TTrackInfo *trackInfo);

private:
};

#endif // TZIPPARSE_H
