#include "preferences.h"

#define SETTING_FILE                "setting.ini"

// General
#define SEC_GUI                     "Gui"
#define SEC_GUI_ALWAYS_ON_TOP       "AlwaysOnTop"
#define SEC_GUI_LANGUAGE            "Language"
#define SEC_GUI_GEOMETRY            "Geometry"
#define SEC_GUI_WINDOW_STATE        "WindowState"
#define SEC_GUI_LAST_OPEN_PATH      "LastOpenPath"
#define SEC_GUI_LAST_OPEN_DIR       "LastOpenDirectory"

// Install
#define SEC_INSTALL                 "Install"
#define SEC_INSTALL_RUN_COUNT       "RunCount"

// Main window
#define SEC_MAIN_WINDOW             "MainWindow"

// Lyric window
#define SEC_LYRIC_WINDOW            "LyricWindow"

// Equalizer window
#define SEC_EQUALIZER_WINDOW        "EqualizerWindow"

// Playlist window
#define SEC_PLAYLIST_WINDOW         "PlaylistWindow"

// Browser window
#define SEC_BROWSER_WINDOW          "BrowserWindow"

// Playlist
#define SEC_CORE                    "Core"
#define SEC_CORE_PLAYLIST_INDEX     "CurrentPlaylist"
#define SEC_CORE_MUSIC_INDEX        "CurrentMusic"
#define SEC_CORE_TRACK_INDEX        "CurrentTrack"

// Options
#define SEC_OPTIONS                 "Options"
#define SEC_OPTION_GENERAL          "OptionGeneral"
#define SEC_OPTION_AUTO_CORRECT     "AutoCorrect"
#define SEC_OPTION_FORCE_CORRECT    "ForceCorrect"

TPreferences *TPreferences::mInstance = NULL;

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
    mSettings->beginGroup(SEC_GUI);
    mAlwaysTop = boolValue(SEC_GUI_ALWAYS_ON_TOP);
    mLanguage = stringValue(SEC_GUI_LANGUAGE);
    mLastOpenPath = stringValue(SEC_GUI_LAST_OPEN_PATH);
    mLastOpenDir = stringValue(SEC_GUI_LAST_OPEN_DIR);
    mSettings->endGroup();

    // Core settings
    mSettings->beginGroup(SEC_CORE);
    mPlayingPlaylistIndex = intValue(SEC_CORE_PLAYLIST_INDEX, -1);
    mPlayingMusicIndex = intValue(SEC_CORE_MUSIC_INDEX, -1);
    mPlayingTrackIndex = intValue(SEC_CORE_TRACK_INDEX, -1);
    mSettings->endGroup();

    //// Options
    mSettings->beginGroup(SEC_OPTIONS);
        // general
        mSettings->beginGroup(SEC_OPTION_GENERAL);
        mAutoCorrectDuration = boolValue(SEC_OPTION_AUTO_CORRECT, true);
        mForceCorrectDuration = boolValue(SEC_OPTION_FORCE_CORRECT);
        mSettings->endGroup();
    mSettings->endGroup();

    //// Keeping track of some usage information
    mSettings->beginGroup(SEC_INSTALL);

    // This section wrote by main controller while write trial license
    if(mSettings->contains(SEC_INSTALL_RUN_COUNT))
    {
        mRunCount = intValue(SEC_INSTALL_RUN_COUNT) + 1;
        mSettings->setValue(SEC_INSTALL_RUN_COUNT, mRunCount);
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

    mSettings->beginGroup(SEC_GUI);
    mSettings->setValue(SEC_GUI_LANGUAGE, mLanguage);
    mSettings->endGroup();
    mLanguage = language;

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
        section = SEC_MAIN_WINDOW;
    else if(type==WT_LYRIC)
        section = SEC_LYRIC_WINDOW;
    else if(type==WT_EQUALIZER)
        section = SEC_EQUALIZER_WINDOW;
    else if(type==WT_PLAYLIST)
        section = SEC_PLAYLIST_WINDOW;
    else if(type==WT_BROWSER)
        section = SEC_BROWSER_WINDOW;
    else
        return;

    mSettings->beginGroup(section);
    *g = mSettings->value(SEC_GUI_GEOMETRY).toByteArray();
    *s = mSettings->value(SEC_GUI_WINDOW_STATE).toByteArray();
    mSettings->endGroup();
}

void TPreferences::setWindowGeometryState(
        TWindowType type,
        QVariant geometry,
        QVariant windowState)
{
    QString section;
    if(type==WT_MAIN)
        section = SEC_MAIN_WINDOW;
    else if(type==WT_LYRIC)
        section = SEC_LYRIC_WINDOW;
    else if(type==WT_EQUALIZER)
        section = SEC_EQUALIZER_WINDOW;
    else if(type==WT_PLAYLIST)
        section = SEC_PLAYLIST_WINDOW;
    else if(type==WT_BROWSER)
        section = SEC_BROWSER_WINDOW;
    else
        return;

    mSettings->beginGroup(section);
    mSettings->setValue(SEC_GUI_GEOMETRY, geometry);
    mSettings->setValue(SEC_GUI_WINDOW_STATE, windowState);
    mSettings->endGroup();
}

QString TPreferences::lastOpenDialogPath()
{
    return mLastOpenPath;
}

QString TPreferences::lastOpenDirectory()
{
    return mLastOpenDir;
}

void TPreferences::setLastOpenDialogPath(QString path)
{
    if(path == mLastOpenPath)
        return;

    mSettings->beginGroup(SEC_GUI);
    mSettings->setValue(SEC_GUI_LAST_OPEN_PATH, path);
    mSettings->endGroup();

    mLastOpenPath = path;
}

void TPreferences::setLastOpenDirectory(QString path)
{
    if(path == mLastOpenDir)
        return;

    mSettings->beginGroup(SEC_GUI);
    mSettings->setValue(SEC_GUI_LAST_OPEN_DIR, path);
    mSettings->endGroup();

    mLastOpenDir = path;
}

int TPreferences::playingPlaylistIndex()
{
    return mPlayingPlaylistIndex;
}

void TPreferences::setPlayingPlaylistIndex(int index)
{
    if(mPlayingPlaylistIndex == index)
        return;

    mSettings->beginGroup(SEC_CORE);
    mSettings->setValue(SEC_CORE_PLAYLIST_INDEX, index);
    mSettings->endGroup();

    mPlayingPlaylistIndex = index;
}

int TPreferences::playingMusicIndex()
{
    return mPlayingMusicIndex;
}

void TPreferences::setPlayingMusicIndex(int index)
{
    if(mPlayingMusicIndex == index)
        return;

    mSettings->beginGroup(SEC_CORE);
    mSettings->setValue(SEC_CORE_MUSIC_INDEX, index);
    mSettings->endGroup();

    mPlayingMusicIndex = index;
}

int TPreferences::playingTrackIndex()
{
    return mPlayingTrackIndex;
}

void TPreferences::setPlayingTrackIndex(int index)
{
    if(mPlayingTrackIndex == index)
        return;

    mSettings->beginGroup(SEC_CORE);
    mSettings->setValue(SEC_CORE_TRACK_INDEX, index);
    mSettings->endGroup();

    mPlayingTrackIndex = index;
}

void TPreferences::autoCorrectDuration()
{

}

void TPreferences::setCorrectDuration(bool autoCorrect)
{

}

void TPreferences::forceCorrectDuration()
{

}

void TPreferences::setForceCorrectDuration(bool force)
{

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
