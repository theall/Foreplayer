/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef SKINLOADER_H
#define SKINLOADER_H

#include "minizip/unzip.h"
#include "skinutils.h"

#include <QDir>
#include <QColor>
#include <QPixmap>
#include <QDomElement>
#include <QXmlStreamReader>

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
    QPixmap preview();

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
    QPixmap mPreview;

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
