#ifndef TFILEPARSE_H
#define TFILEPARSE_H

#include "abstractparse.h"

class TFileParse : public TAbstractParse
{
public:
    explicit TFileParse(QString file = QString());

    // TAbstractParse interface
    bool parse(TMusicInfo *musicInfo) Q_DECL_OVERRIDE;

    // Parse file data of rsn package
    TTrackInfo *parse(QByteArray data);

private:
};

#endif // TFILEPARSE_H
