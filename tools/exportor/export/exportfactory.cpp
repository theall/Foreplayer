#include "exportfactory.h"

#include <QString>
#include <QFileInfo>

TExportFactory::TExportFactory()
{

}

TExportFactory::~TExportFactory()
{
    recycleAll();
}

TAbstractExport *TExportFactory::create(ExportFormat format, const wchar_t *fileName, int sampleRate)
{
    TAbstractExport *newProduct = NULL;
    switch (format) {
    case EF_MP3:
        newProduct = new TMP3Export(fileName, sampleRate);
        break;
    case EF_PCM:
        newProduct = new TPcmExport(fileName, sampleRate);
        break;
    default:
        newProduct = new TWaveExport(fileName, sampleRate);
        break;
    }
    mProducts.append(newProduct);
    return newProduct;
}

TAbstractExport *TExportFactory::create(QString suffix, const wchar_t *fileName, int sampleRate)
{
    ExportFormat ef = EF_WAVE;
    suffix = suffix.toLower();
    if(suffix == "mp3")
        ef = EF_MP3;
    else if(suffix == "pcm")
        ef = EF_PCM;

    return create(ef, fileName, sampleRate);
}

void TExportFactory::recycle(TAbstractExport *product)
{
    int pSize = mProducts.size();
    if(pSize <= 0)
        return;

    int index = pSize - 1;
    int pIndex = mProducts.indexOf(product);
    if(pIndex >= 0)
        index = pIndex;

    delete mProducts.takeAt(index);
}

void TExportFactory::recycleAll()
{
    foreach (TAbstractExport *p, mProducts) {
        if(p)
            delete p;
    }
    mProducts.clear();
}

TAbstractExport *TExportFactory::create(const wchar_t *fileName, int sampleRate)
{
    QFileInfo fi(QString::fromWCharArray(fileName));
    return create(fi.suffix(), fileName, sampleRate);
}

