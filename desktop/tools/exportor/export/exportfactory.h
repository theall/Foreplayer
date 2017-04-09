#ifndef TEXPORTFACTORY_H
#define TEXPORTFACTORY_H

#include "waveexport.h"
#include "mp3export.h"
#include "pcmexport.h"

#include <QVector>

enum ExportFormat
{
    EF_WAVE,
    EF_MP3,
    EF_PCM
};

class TExportFactory
{
public:
    TExportFactory();
    ~TExportFactory();

    TAbstractExport *create(const wchar_t *fileName, int sampleRate);
    TAbstractExport *create(ExportFormat format, const wchar_t *fileName, int sampleRate);
    TAbstractExport *create(QString suffix, const wchar_t *fileName, int sampleRate);

    void recycle(TAbstractExport *product=NULL);
    void recycleAll();

private:
    QVector<TAbstractExport*> mProducts;
};

#endif // TEXPORTFACTORY_H
