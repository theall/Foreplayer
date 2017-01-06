#include "skin.h"

#include <QTemporaryFile>

#define BUF_SIZE                    16384

#define TEXT(x) x.toLatin1().constData()

TSkin::TSkin(QObject *parent) : QObject(parent)
  , mZipfile(NULL)
{

}

TSkin::~TSkin()
{
    if(mZipfile)
    {
        unzClose(mZipfile);
        mZipfile = NULL;
    }
}

bool TSkin::load(QString fileName)
{
    if(fileName.isEmpty())
        return false;

    QFileInfo fi(fileName);
    QString fileSuffix = fi.suffix().toLower();

    bool success = false;
    mError.clear();

    if(fileSuffix=="xml")
        success = loadFromXmlFile(fileName);
    else
        success = loadFromZipFile(fileName);

    if(success)
    {
        mFileName = fileName;
        mFileDir = QFileInfo(fileName).dir();
    } else {
        qDebug() << mError;
    }

    return success;
}

void TSkin::save()
{
    if(mFileName.isEmpty())
        return;

    QFile file(mFileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

    QTextStream textStream(&file);
    QDomDocument document;
    QDomElement skinElement = document.createElement(TAG_SKIN);
    skinElement.setAttribute(ATTR_VERSION, mVersion);
    skinElement.setAttribute(ATTR_NAME, mName);
    skinElement.setAttribute(ATTR_AUTHOR, mAuthor);
    skinElement.setAttribute(ATTR_URL, mUrl);
    skinElement.setAttribute(ATTR_EMAIL, mEmail);
    skinElement.setAttribute(ATTR_TRANSPARENT_COLOR, mTransparentColor.name());

    document.save(textStream, 4);
}

void TSkin::saveTo(QString fileName)
{
    if(!fileName.isEmpty())
        return;

    mFileName = fileName;
    save();
}

QDomElement TSkin::rootElement()
{
    return mRootElement;
}

QPixmap TSkin::findPixmap(QString fileName)
{
    QPixmap pixmap;
    if(mZipfile)
        pixmap = readPixmapFromZip(fileName);
    else
        pixmap.load(mFileDir.absoluteFilePath(fileName));

    QBitmap mask = pixmap.createMaskFromColor(mTransparentColor, Qt::MaskInColor);
    pixmap.setMask(mask);

    return pixmap;
}

QIcon TSkin::findIcon(QString fileName)
{
    if(mZipfile)
        return readIconFromZip(fileName);
    return QIcon(mFileDir.absoluteFilePath(fileName));
}

bool TSkin::loadFromXmlFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        mError = tr("Open file %1 failed, error: %2").arg(fileName).arg(file.errorString());
        return false;
    }

    bool status = parseXmlDocument(file.readAll());

    file.close();

    return status;
}

bool TSkin::loadFromZipFile(QString fileName)
{
    mZipfile = unzOpen(TEXT(fileName));
    if(!mZipfile)
    {
        mError = tr("Fail to open zip file %1").arg(fileName);
        return false;
    }

    QByteArray byteArray;
    if(!readFileFromZip(ZIP_SKIN_NAME, byteArray))
    {
        mError = tr("Fail to open xml file %1").arg(fileName);
        return false;
    }

    return parseXmlDocument(byteArray);
}

bool TSkin::parseXmlDocument(const QByteArray &byteArray)
{
    QDomDocument document;
    QString strError;
    int errLine = 0;
    int errColumn = 0;
    if(!document.setContent(byteArray, false, &strError, &errLine, &errColumn)) {
        mError = tr("Parse file failed at line %1, column %2, %3").arg(errLine).arg(errColumn).arg(strError);
        return false;
    }

    if( document.isNull()) {
        mError = tr("document is null !");
        return false;
    }

    mRootElement = document.documentElement();
    if(mRootElement.tagName() != TAG_SKIN)
    {
        mError = tr("Invalid first child element!It should be %1").arg(TAG_SKIN);
        return false;
    }
    if(mRootElement.isNull())
    {
        mError = tr("Skin element is null!");
        return false;
    }
    mVersion     = mRootElement.attribute(ATTR_VERSION);
    mName        = mRootElement.attribute(ATTR_NAME);
    mAuthor      = mRootElement.attribute(ATTR_AUTHOR);
    mUrl         = mRootElement.attribute(ATTR_URL);
    mEmail       = mRootElement.attribute(ATTR_EMAIL);
    mTransparentColor = QColor(mRootElement.attribute(ATTR_TRANSPARENT_COLOR));

    return true;
}

QPixmap TSkin::readPixmapFromZip(QString fileName)
{
    QByteArray array;
    QPixmap p;
    if(readFileFromZip(fileName, array))
        p.loadFromData(array);

    return p;
}

QIcon TSkin::readIconFromZip(QString fileName)
{
    QIcon ico;
    QByteArray array;
    if(readFileFromZip(fileName, array))
    {
        QPixmap p;
        p.loadFromData(array);
        ico.addPixmap(p);
    }
    return ico;
}

bool TSkin::readFileFromZip(QString fileName, QByteArray &byteArray)
{
    if(!mZipfile)
    {
        mError = tr("Zip file is not opened.");
        return false;
    }
    int status = unzLocateFile(mZipfile, TEXT(fileName), 0);
    if(status != UNZ_OK)
    {
        mError = tr("Fail to locate file %1").arg(ZIP_SKIN_NAME);
        return false;
    }
    status = unzOpenCurrentFile(mZipfile);
    if(status != UNZ_OK)
    {
        mError = tr("Fail to open file %1").arg(ZIP_SKIN_NAME);
        return false;
    }

    char buf[BUF_SIZE];
    int readLength = 0;
    while(true)
    {
        readLength = unzReadCurrentFile(mZipfile, buf, BUF_SIZE);
        if(readLength==UNZ_ERRNO)
        {
            mError = tr("IO error.");
            return false;
        } else if (readLength==0) {
            break;
        } else if (readLength>0) {
            byteArray.append(buf, readLength);
        }
    }
    unzCloseCurrentFile(mZipfile);

    return true;
}

void TSkin::saveAsXml()
{

}

void TSkin::saveAsZip()
{

}
