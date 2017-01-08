#include "preferences.h"

#define SETTING_FILE "setting.ini"

TPreferences *TPreferences::mInstance=NULL;

//
// This class holds user preferences and provides a convenient interface to
// access them.
//
TPreferences::TPreferences(QObject *parent):
    QObject(parent)
{
    QDir dir(qApp->applicationDirPath());
    mSettings = new QSettings(dir.absoluteFilePath(SETTING_FILE), QSettings::IniFormat);

    // Retrieve gui settings
    mSettings->beginGroup("Gui");
    mAlwaysTop = boolValue("AlwaysOnTop");
    mLanguage = stringValue("Language");
    mSettings->endGroup();

    // Keeping track of some usage information
    mSettings->beginGroup("Install");

    // This section wrote by main controller while write trial license
    if(mSettings->contains("RunCount"))
    {
        mRunCount = intValue("RunCount") + 1;
        mSettings->setValue("RunCount", mRunCount);
    }

    mSettings->endGroup();
}

TPreferences::~TPreferences()
{
    if(mSettings)
    {
        mSettings->sync();
        delete mSettings;
        mSettings = NULL;
    }
}

TPreferences* TPreferences::instance()
{
    if(mInstance==NULL)
        mInstance = new TPreferences();
    return mInstance;
}

void TPreferences::deleteInstance()
{
    if(mInstance)
    {
        delete mInstance;
        mInstance = NULL;
    }
}

QString TPreferences::language()
{
    return mLanguage;
}

void TPreferences::setLanguage(QString language)
{
    if((mLanguage == language))
        return;
    mLanguage = language;
    mSettings->setValue("Gui/Language", mLanguage);
    emit languageChanged();
}

int TPreferences::runCount()
{
    return mRunCount;
}

void TPreferences::windowGeometryState(
        TWindowType type,
        QByteArray* g,
        QByteArray* s)
{
    QString section;
    if(type==WT_MAIN)
        section = "MainWindow";
    else if(type==WT_LYRIC)
        section = "LyricWindow";
    else if(type==WT_EQUALIZER)
        section = "EqualizerWindow";
    else if(type==WT_PLAYLIST)
        section = "PlaylistWindow";
    else if(type==WT_BROWSER)
        section = "BrowserWindow";
    else
        return;

    mSettings->beginGroup(section);
    *g = mSettings->value("geometry").toByteArray();
    *s = mSettings->value("windowState").toByteArray();
    mSettings->endGroup();
}

void TPreferences::setWindowGeometryState(
        TWindowType type,
        QVariant geometry,
        QVariant windowState)
{
    QString section;
    if(type==WT_MAIN)
        section = "MainWindow";
    else if(type==WT_LYRIC)
        section = "LyricWindow";
    else if(type==WT_EQUALIZER)
        section = "EqualizerWindow";
    else if(type==WT_PLAYLIST)
        section = "PlaylistWindow";
    else if(type==WT_BROWSER)
        section = "BrowserWindow";
    else
        return;

    mSettings->beginGroup(section);
    mSettings->setValue("geometry", geometry);
    mSettings->setValue("windowState", windowState);
    mSettings->endGroup();
}

void TPreferences::setValue(QString section, QVariant value)
{
    mSettings->setValue(section, value);
}

QVariant TPreferences::value(QString section, QVariant defValue)
{
    return mSettings->value(section, defValue);
}

bool TPreferences::boolValue(QString key, bool defValue)
{
    return mSettings->value(key, defValue).toBool();
}

QColor TPreferences::colorValue(QString key, QColor defValue)
{
    QString name = mSettings->value(key, defValue.name()).toString();
    if((!QColor::isValidColor(name)))
        return QColor();
    return QColor(name);
}

QString TPreferences::stringValue(QString key, QString defValue)
{
    return mSettings->value(key, defValue).toString();
}

int TPreferences::intValue(QString key, int defaultValue)
{
    bool ok = false;
    int v = mSettings->value(key, defaultValue).toInt(&ok);
    if (ok)
        return v;
    return defaultValue;
}

float TPreferences::floatValue(QString key, float defaultValue)
{
    bool ok = false;
    float v = mSettings->value(key, defaultValue).toFloat(&ok);
    if (ok)
        return v;
    return defaultValue;
}

QStringList TPreferences::listValue(QString key)
{
    QList<QVariant> list = mSettings->value(key).toList();
    QStringList r;
    foreach (QVariant v, list) {
        r.append(v.toString());
    }
    return r;
}

QDate TPreferences::dateValue(QString key, int defaultValue)
{
    if (defaultValue==-1)
        defaultValue = QDate::currentDate().toJulianDay();
    int days = intValue(key, defaultValue);
    return QDate::fromJulianDay(days);
}

QTime TPreferences::timeValue(QString key, int defaultValue)
{
    int seconds = intValue(key, defaultValue);
    QTime time = QTime::fromMSecsSinceStartOfDay(seconds);
    if(!time.isValid())
        time = QTime::fromMSecsSinceStartOfDay(0);
    return time;
}
