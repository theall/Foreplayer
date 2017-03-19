#ifndef SKINLOADER_H
#define SKINLOADER_H

#include <unzip/unzip.h>
#include "skinutils.h"

class TSkin : public QObject
{
    Q_OBJECT

public:
    explicit TSkin(QObject *parent = 0);
    ~TSkin();

    bool load(QString fileName);
    void save();
    void saveTo(QString fileName);

    QDomElement rootElement();

    QPixmap findPixmap(QString fileName);
    QIcon findIcon(QString fileName);

    void addFile(QString diskFile, QString zipFile);

    QString version() { return mVersion; }
    QString name() { return mName; }
    QString author() { return mAuthor; }
    QString url() { return mUrl; }
    QString email() { return mEmail; }
    QString fileName() { return mFileName; }
    QColor transparentColor() { return mTransparentColor; }
    QString lastError() { return mError; }

private:
    QString mVersion;
    QString mName;
    QString mAuthor;
    QString mUrl;
    QString mEmail;
    QColor mTransparentColor;
    QString mFileName;
    QDir mFileDir;
    QString mError;

    QDomElement mRootElement;

    unzFile mZipfile;

    bool loadFromXmlFile(QString fileName);
    bool loadFromXmlStream();
    bool loadFromZipFile(QString fileName);
    bool parseXmlDocument(const QByteArray &byteArray);

    QPixmap readPixmapFromZip(QString fileName);
    QIcon readIconFromZip(QString fileName);

    bool readFileFromZip(QString fileName, QByteArray &byteArray);

    void saveAsXml();
    void saveAsZip();

    void readSkinAttr(QXmlStreamReader &xml);
    void writeSkinAttr(QXmlStreamWriter &xml);
};

class TSkinReader
{
public:
    virtual void loadFromSkin(QDomElement element, TSkin *skin) = 0;
};

class TSkinWriter : public TSkinReader
{
public:
    virtual void saveToSkin(QDomElement element, TSkin skin) = 0;
};


#endif // SKINLOADER_H
