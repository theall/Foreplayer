#ifndef TABSTRACTPARSE_H
#define TABSTRACTPARSE_H

#include "../backendinterface.h"

#include <QString>
#include "gmewrap.h"

class TAbstractParse
{
public:
    explicit TAbstractParse(QString file);
    ~TAbstractParse();

    virtual bool parse(TMusicInfo* musicInfo) = 0;

protected:
    QString mFile;
};

#endif // TABSTRACTPARSE_H
