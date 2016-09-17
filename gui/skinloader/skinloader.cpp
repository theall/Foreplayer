#include "skinloader.h"

#define TAG_PLAYER_WINDOW "player_window"
#define TAG_LYRIC_WINDOW "lyric_window"
#define TAG_EQUALIZER_WINDOW "equalizer_window"
#define TAG_PLAYLIST_WINDOW "playlist_window"
#define TAG_MINI_WINDOW "mini_window"
#define TAG_BROWSER_WINDOW "browser_window"
#define TAG_DESKLRC_BAR "desklrc_bar"

SkinLoader::SkinLoader(QObject *parent) : QObject(parent)
{

}

PlayerWindowParam *SkinLoader::mainWindow()
{
    return &mPlayerWindow;
}

LyricWindowParam *SkinLoader::lyricWindow()
{
    return &mLyricWIndow;
}

EqualizerWindowParam *SkinLoader::equalizerWindow()
{
    return &mEqualizerWindow;
}

PlaylistWindowParam *SkinLoader::playlistWindow()
{
    return &mPlaylistWindow;
}

PlayerWindowParam *SkinLoader::miniWindow()
{
    return &mMiniWindow;
}

BrowserWindowParam *SkinLoader::browserWindow()
{
    return &mBrowserWindow;
}

DesklrcBarParam *SkinLoader::desklrcBar()
{
    return &mDesklrcBar;
}

void SkinLoader::load(QString fileName)
{
    if(fileName.isEmpty())
            return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QFileInfo fi(file);
    mSkinPath = fi.absolutePath();

    QXmlStreamReader xml(&file);

    while(!xml.atEnd() && !xml.hasError())
    {
        //读取下一个element.
        QXmlStreamReader::TokenType token = xml.readNext();

        //如果获取的仅为StartDocument,则进行下一个
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }

        //如果获取了StartElement,则尝试读取
        if(token == QXmlStreamReader::StartElement)
        {
            QStringRef name = xml.name();
            if(name == "skin")
            {
                parseSkinNode(xml);
            }
            else if(name == TAG_PLAYER_WINDOW)
            {
                parsePlayerWindow(xml);
            }
            else if(name == TAG_LYRIC_WINDOW)
            {
                parseLyricWindow(xml);
            }
            else if(name == TAG_EQUALIZER_WINDOW)
            {
                parseEqualizerWindow(xml);
            }
            else if(name == TAG_PLAYLIST_WINDOW)
            {
                parsePlaylistWindow(xml);
            }
            else if(name == TAG_MINI_WINDOW)
            {
                parseMiniWindow(xml);
            }
            else if(name == TAG_BROWSER_WINDOW)
            {
                parseBrowserWindow(xml);
            }
            else if(name == TAG_DESKLRC_BAR)
            {
                parseDesklrcBar(xml);
            }
        }
        else if(token==QXmlStreamReader::Invalid)
        {
            break;
        }
    }

    if(xml.hasError())
    {
        qDebug() << "Invalid xml file.";
    }

    //从reader中删除所有设备、数据，并将其重置为初始状态
    xml.clear();
}

void SkinLoader::parseSkinNode(QXmlStreamReader& xml)
{
    //get attr of skin
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("version"))
    {
        m_skinAttr.version = attributes.value("version").toString();
    }
    if(attributes.hasAttribute("name"))
    {
        m_skinAttr.name = attributes.value("name").toString();
    }
    if(attributes.hasAttribute("author"))
    {
        m_skinAttr.author = attributes.value("author").toString();
    }
    if(attributes.hasAttribute("url"))
    {
        m_skinAttr.url = attributes.value("url").toString();
    }
    if(attributes.hasAttribute("email"))
    {
        m_skinAttr.email = attributes.value("email").toString();
    }
    if(attributes.hasAttribute("transparent_color"))
    {
        m_skinAttr.transparent = QColor(attributes.value("transparent_color").toString());
    }
}

void SkinLoader::parsePlayerWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_PLAYER_WINDOW)
        return;

    parseWindowParam(xml, mPlayerWindow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="play")
            {
                parseElement(xml, mPlayerWindow.play);
            }
            else if(name=="pause")
            {
                parseElement(xml, mPlayerWindow.pause);
            }
            else if(name=="prev")
            {
                parseElement(xml, mPlayerWindow.prev);
            }
            else if(name=="next")
            {
                parseElement(xml, mPlayerWindow.next);
            }
            else if(name=="stop")
            {
                parseElement(xml, mPlayerWindow.stop);
            }
            else if(name=="open")
            {
                parseElement(xml, mPlayerWindow.open);
            }
            else if(name=="mute")
            {
                parseElement(xml, mPlayerWindow.mute);
            }
            else if(name=="lyric")
            {
                parseElement(xml, mPlayerWindow.lyric);
            }
            else if(name=="equalizer")
            {
                parseElement(xml, mPlayerWindow.equalizer);
            }
            else if(name=="playlist")
            {
                parseElement(xml, mPlayerWindow.playlist);
            }
            else if(name=="browser")
            {
                parseElement(xml, mPlayerWindow.browser);
            }
            else if(name=="minimize")
            {
                parseElement(xml, mPlayerWindow.minimize);
            }
            else if(name=="minimode")
            {
                parseElement(xml, mPlayerWindow.minimode);
            }
            else if(name=="exit")
            {
                parseElement(xml, mPlayerWindow.exit);
            }
            else if(name=="progress")
            {
                parseElement(xml, mPlayerWindow.progress);
            }
            else if(name=="volume")
            {
                parseElement(xml, mPlayerWindow.volume);
            }
            else if(name=="icon")
            {
                parseElement(xml, mPlayerWindow.icon);
            }
            else if(name=="info")
            {
                parseElement(xml, mPlayerWindow.info);
            }
            else if(name=="led")
            {
                parseElement(xml, mPlayerWindow.led);
            }
            else if(name=="stereo")
            {
                parseElement(xml, mPlayerWindow.stereo);
            }
            else if(name=="status")
            {
                parseElement(xml, mPlayerWindow.status);
            }
            else if(name=="visual")
            {
                parseElement(xml, mPlayerWindow.visual);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_PLAYER_WINDOW)
                break;
        }
    }
}

void SkinLoader::parseLyricWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_LYRIC_WINDOW)
        return;

    parseWindowParam(xml, mLyricWIndow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="lyric")
            {
                parseElement(xml, mLyricWIndow.minimize);
            }
            else if(name=="close")
            {
                parseElement(xml, mLyricWIndow.close);
            }
            else if(name=="ontop")
            {
                parseElement(xml, mLyricWIndow.ontop);
            }
            else if(name=="desklrc")
            {
                parseElement(xml, mLyricWIndow.desklyc);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_LYRIC_WINDOW)
                break;
        }
    }
}

void SkinLoader::parseEqualizerWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_EQUALIZER_WINDOW)
        return;

    parseWindowParam(xml, mEqualizerWindow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="close")
            {
                parseElement(xml, mEqualizerWindow.close);
            }
            else if(name=="enabled")
            {
                parseElement(xml, mEqualizerWindow.enabled);
            }
            else if(name=="profile")
            {
                parseElement(xml, mEqualizerWindow.profile);
            }
            else if(name=="reset")
            {
                parseElement(xml, mEqualizerWindow.reset);
            }
            else if(name=="balance")
            {
                parseElement(xml, mEqualizerWindow.balance);
            }
            else if(name=="surround")
            {
                parseElement(xml, mEqualizerWindow.surround);
            }
            else if(name=="preamp")
            {
                parseElement(xml, mEqualizerWindow.preamp);
            }
            else if(name=="eqfactor")
            {
                parseElement(xml, mEqualizerWindow.eqfactor);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_EQUALIZER_WINDOW)
                break;
        }
    }
}

void SkinLoader::parsePlaylistWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_PLAYLIST_WINDOW)
        return;

    parseWindowParam(xml, mPlaylistWindow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="close")
            {
                parseElement(xml, mPlaylistWindow.close);
            }
            else if(name=="toolbar")
            {
                parseElement(xml, mPlaylistWindow.toolbar);
            }
            else if(name=="scrollbar")
            {
                parseElement(xml, mPlaylistWindow.scrollbar);
            }
            else if(name=="playlist")
            {
                parseElement(xml, mPlaylistWindow.playlist);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_PLAYLIST_WINDOW)
                break;
        }
    }
}

void SkinLoader::parseMiniWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_MINI_WINDOW)
        return;

    parseWindowParam(xml, mMiniWindow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="play")
            {
                parseElement(xml, mMiniWindow.play);
            }
            else if(name=="pause")
            {
                parseElement(xml, mMiniWindow.pause);
            }
            else if(name=="prev")
            {
                parseElement(xml, mMiniWindow.prev);
            }
            else if(name=="next")
            {
                parseElement(xml, mMiniWindow.next);
            }
            else if(name=="stop")
            {
                parseElement(xml, mMiniWindow.stop);
            }
            else if(name=="open")
            {
                parseElement(xml, mMiniWindow.open);
            }
            else if(name=="mute")
            {
                parseElement(xml, mMiniWindow.mute);
            }
            else if(name=="lyric")
            {
                parseElement(xml, mMiniWindow.lyric);
            }
            else if(name=="minimode")
            {
                parseElement(xml, mMiniWindow.minimode);
            }
            else if(name=="minimize")
            {
                parseElement(xml, mMiniWindow.minimize);
            }
            else if(name=="exit")
            {
                parseElement(xml, mMiniWindow.exit);
            }
            else if(name=="icon")
            {
                parseElement(xml, mMiniWindow.icon);
            }
            else if(name=="led")
            {
                parseElement(xml, mMiniWindow.led);
            }
            else if(name=="info")
            {
                parseElement(xml, mMiniWindow.info);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_MINI_WINDOW)
                break;
        }
    }
}

void SkinLoader::parseBrowserWindow(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_BROWSER_WINDOW)
        return;

    parseWindowParam(xml, mBrowserWindow.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="close")
            {
                parseElement(xml, mBrowserWindow.close);
            }
            else if(name=="backward")
            {
                parseElement(xml, mBrowserWindow.backward);
            }
            else if(name=="forward")
            {
                parseElement(xml, mBrowserWindow.forward);
            }
            else if(name=="refresh")
            {
                parseElement(xml, mBrowserWindow.refresh);
            }
            else if(name=="startup")
            {
                parseElement(xml, mBrowserWindow.startup);
            }
            else if(name=="linktxt")
            {
                parseElement(xml, mBrowserWindow.linktxt);
            }
            else if(name=="browser")
            {
                parseElement(xml, mBrowserWindow.browser);
            }
            else if(name=="minibtn")
            {
                parseElement(xml, mBrowserWindow.minibtn);
            }
            else if(name=="downloadbtn")
            {
                parseElement(xml, mBrowserWindow.downloadbtn);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_BROWSER_WINDOW)
                break;
        }
    }
}

void SkinLoader::parseDesklrcBar(QXmlStreamReader &xml)
{
    if(xml.name() != TAG_DESKLRC_BAR)
        return;

    parseWindowParam(xml, mDesklrcBar.window);

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();
        QStringRef name = xml.name();
        if(token == QXmlStreamReader::StartElement)
        {
            if(name=="icon")
            {
                parseElement(xml, mDesklrcBar.icon);
            }
            else if(name=="play")
            {
                parseElement(xml, mDesklrcBar.play);
            }
            else if(name=="pause")
            {
                parseElement(xml, mDesklrcBar.pause);
            }
            else if(name=="prev")
            {
                parseElement(xml, mDesklrcBar.prev);
            }
            else if(name=="next")
            {
                parseElement(xml, mDesklrcBar.next);
            }
            else if(name=="list")
            {
                parseElement(xml, mDesklrcBar.list);
            }
            else if(name=="settings")
            {
                parseElement(xml, mDesklrcBar.settings);
            }
            else if(name=="kalaok")
            {
                parseElement(xml, mDesklrcBar.kalaok);
            }
            else if(name=="lines")
            {
                parseElement(xml, mDesklrcBar.lines);
            }
            else if(name=="lock")
            {
                parseElement(xml, mDesklrcBar.lock);
            }
            else if(name=="ontop")
            {
                parseElement(xml, mDesklrcBar.ontop);
            }
            else if(name=="return")
            {
                parseElement(xml, mDesklrcBar.back);
            }
            else if(name=="close")
            {
                parseElement(xml, mDesklrcBar.close);
            }
        }
        else if(token == QXmlStreamReader::EndElement)
        {
            if(name==TAG_DESKLRC_BAR)
                break;
        }
    }
}

QPixmap SkinLoader::loadPixmap(QString imageName)
{
    QString filePath = mSkinPath + "/" + imageName;
    QPixmap pixmap(filePath);
    QBitmap mask = pixmap.createMaskFromColor(m_skinAttr.transparent, Qt::MaskInColor);
    pixmap.setMask(mask);
    return pixmap;
}

QIcon SkinLoader::loadIcon(QString iconFile)
{
    QString filePath = mSkinPath + "/" + iconFile;
    return QIcon(filePath);
}

void SkinLoader::parseElement(QXmlStreamReader &xml, Element &element)
{
    //get attr of skin
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("icon"))
    {
        element.icon = loadIcon(attributes.value("icon").toString());
    }
    if(attributes.hasAttribute("image"))
    {
        element.image = loadPixmap(attributes.value("image").toString());
    }
    if(attributes.hasAttribute("ckbox_image"))
    {
        element.image = loadPixmap(attributes.value("ckbox_image").toString());
    }
    if(attributes.hasAttribute("position"))
    {
        element.position = TPosition(attributes.value("position").toString());
    }
    if(attributes.hasAttribute("align"))
    {
        element.alignment = str2Align(attributes.value("align").toString());
    }
    if(attributes.hasAttribute("bar_image"))
    {
        element.image = loadPixmap(attributes.value("bar_image").toString());
    }
    if(attributes.hasAttribute("thumb_image"))
    {
        element.image1 = loadPixmap(attributes.value("thumb_image").toString());
    }
    if(attributes.hasAttribute("hot_image"))
    {
        element.image1 = loadPixmap(attributes.value("hot_image").toString());
    }
    if(attributes.hasAttribute("fill_image"))
    {
        element.image2 = loadPixmap(attributes.value("fill_image").toString());
    }
    if(attributes.hasAttribute("buttons_image"))
    {
        element.image2 = loadPixmap(attributes.value("buttons_image").toString());
    }
    if(attributes.hasAttribute("color"))
    {
        element.color = QColor(attributes.value("color").toString());
    }
    if(attributes.hasAttribute("font"))
    {
        element.font.setFamily(attributes.value("font").toString());
    }
    if(attributes.hasAttribute("font_size"))
    {
        element.font.setPixelSize(attributes.value("font_size").toInt());
    }
    if(attributes.hasAttribute("vertical"))
    {
        element.vertical = str2bool(attributes.value("vertical").toString());
    }
    if(attributes.hasAttribute("thumb_resize_center"))
    {
        element.interval = str2bool(attributes.value("thumb_resize_center").toString());
    }
}

void SkinLoader::parseWindowParam(QXmlStreamReader &xml, WindowParam &window)
{
    //get attr of skin
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("image"))
    {
        window.background = loadPixmap(attributes.value("image").toString());
    }
    if(attributes.hasAttribute("position"))
    {
        window.position = TPosition(attributes.value("position").toString());
    }
    if(attributes.hasAttribute("resize_rect"))
    {
        window.resize = TPosition(attributes.value("resize_rect").toString());
    }
    if(attributes.hasAttribute("eq_interval"))
    {
        window.interval = attributes.value("eq_interval").toInt();
    }
    if(attributes.hasAttribute("transparent_color"))
    {
        window.transparent = QColor(attributes.value("transparent_color").toString());
    }
}
