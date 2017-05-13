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
#include "preferences.h"
#include "utils.h"

#define SETTING_FILE                "setting.ini"

// General
#define SEC_GUI                     "Gui"
#define SEC_GUI_ALWAYS_ON_TOP       "AlwaysOnTop"
#define SEC_GUI_LANGUAGE            "Language"
#define SEC_GUI_GEOMETRY            "Geometry"
#define SEC_GUI_WINDOW_STATE        "WindowState"
#define SEC_GUI_LAST_OPEN_PATH      "LastOpenPath"
#define SEC_GUI_LAST_OPEN_DIR       "LastOpenDirectory"
#define SEC_GUI_LAST_SKIN_PATH      "LastSkinPath"
#define SEC_GUI_OPACITY             "Opacity"

// Install
#define SEC_INSTALL                 "Install"
#define SEC_INSTALL_RUN_COUNT       "RunCount"

// Main window
#define SEC_MAIN_WINDOW                 "MainWindow"
#define SEC_MAIN_WINDOW_MUTE            "Mute"
#define SEC_MAIN_WINDOW_VOLUME          "Volume"
#define SEC_MAIN_WINDOW_LYRIC_BTN       "LyricChecked"
#define SEC_MAIN_WINDOW_EQ_BTN          "EqualizerChecked"
#define SEC_MAIN_WINDOW_PLAYLIST_BTN    "PlaylistChecked"
#define SEC_MAIN_WINDOW_PLAY_MODE       "PlayMode"

// Lyric window
#define SEC_LYRIC_WINDOW                "LyricWindow"

// Equalizer window
#define SEC_EQUALIZER_WINDOW            "EqualizerWindow"
#define SEC_EQUALIZER_WINDOW_ENABLED    "Enabled"
#define SEC_EQUALIZER_WINDOW_BALLANCE   "Ballance"
#define SEC_EQUALIZER_WINDOW_SURROUND   "Surround"
#define SEC_EQUALIZER_WINDOW_AMPLIFY    "Amplification"
#define SEC_EQUALIZER_WINDOW_FACTORS    "Factors"
#define SEC_EQUALIZER_WINDOW_PROFILE    "Profile"

// Playlist window
#define SEC_PLAYLIST_WINDOW         "PlaylistWindow"

// Browser window
#define SEC_BROWSER_WINDOW          "BrowserWindow"

// Options
#define SEC_OPTIONS                 "Options"

#define SEC_OPTION_GENERAL              "OptionGeneral"
#define SEC_OPTION_AUTO_CORRECT         "AutoCorrect"
#define SEC_OPTION_FORCE_CORRECT        "ForceCorrect"
#define SEC_OPTION_AUTO_PLAY            "AutoPlay"
#define SEC_OPTION_DISPLAY_TRAY_ICON    "DisplayTrayIcon"
#define SEC_OPTION_MULTI_INSTANCE       "MultiInstance"
#define SEC_OPTION_PILOT_DURATION       "PilotDuration"
#define SEC_OPTION_CHECK_DURATION       "CheckDuration"
#define SEC_OPTION_EXPORT_PROCESSES     "ExportProcesses"
#define SEC_OPTION_EXPORT_AUTO_CLEAR    "AutoClearCompletedMissions"

#define SET_VALUE(value,member,parent,section) \
    if(member==value)\
        return;\
    mSettings->beginGroup(parent);\
    mSettings->setValue(section, value);\
    mSettings->endGroup();\
    member = value

#define SET_VALUE2(value,member,parent,sub,section) \
    if(member==value)\
        return;\
    mSettings->beginGroup(parent);\
    mSettings->beginGroup(sub);\
    mSettings->setValue(section, value);\
    mSettings->endGroup();\
    mSettings->endGroup();\
    member = value

TPreferences *TPreferences::mInstance = NULL;

//
// This class holds user preferences and provides a convenient interface to
// access them.
//
TPreferences::TPreferences(QObject *parent):
    QObject(parent)
{
    mSettings = new QSettings(Utils::absoluteFilePath(SETTING_FILE), QSettings::IniFormat);

    // Retrieve gui settings
    mSettings->beginGroup(SEC_GUI);
    mAlwaysTop = boolValue(SEC_GUI_ALWAYS_ON_TOP);
    mLanguage = stringValue(SEC_GUI_LANGUAGE);
    mLastOpenPath = stringValue(SEC_GUI_LAST_OPEN_PATH);
    mLastOpenDir = stringValue(SEC_GUI_LAST_OPEN_DIR);
    mSkinPath = stringValue(SEC_GUI_LAST_SKIN_PATH);
    mOpacity = floatValue(SEC_GUI_OPACITY, 1.0);
    mSettings->endGroup();

    // Main window
    mSettings->beginGroup(SEC_MAIN_WINDOW);
    mMuteEnabled = boolValue(SEC_MAIN_WINDOW_MUTE);
    mVolumeValue = intValue(SEC_MAIN_WINDOW_VOLUME, 100);
    mLyricWindowVisible = boolValue(SEC_MAIN_WINDOW_LYRIC_BTN);
    mEqWindowVisible = boolValue(SEC_MAIN_WINDOW_EQ_BTN, true);
    mPlaylistWindowVisible = boolValue(SEC_MAIN_WINDOW_PLAYLIST_BTN, true);
    mPlayMode = (PlayMode)intValue(SEC_MAIN_WINDOW_PLAY_MODE, (int)MANUAL);
    mSettings->endGroup();

    // Equalizer window
    mSettings->beginGroup(SEC_EQUALIZER_WINDOW);
    mEqEnabled = boolValue(SEC_EQUALIZER_WINDOW_ENABLED);
    mEqBallance = intValue(SEC_EQUALIZER_WINDOW_BALLANCE);
    mEqSurround = intValue(SEC_EQUALIZER_WINDOW_SURROUND);
    mEqAmplification = intValue(SEC_EQUALIZER_WINDOW_AMPLIFY);
    mEqProfile = intValue(SEC_EQUALIZER_WINDOW_PROFILE);
    QString factors = stringValue(SEC_EQUALIZER_WINDOW_FACTORS);
    QStringList sl = factors.split(",");
    for(QString s : sl)
        mEqFactors.append(s.toInt());
    if(mEqFactors.size() != 10)
    {
        mEqFactors.clear();
        for(int i=0;i<10;i++)
            mEqFactors.append(0);
    }
    mSettings->endGroup();

    //// Options
    mSettings->beginGroup(SEC_OPTIONS);
        // general
        mSettings->beginGroup(SEC_OPTION_GENERAL);
        mAutoCorrectDuration = boolValue(SEC_OPTION_AUTO_CORRECT, true);
        mForceCorrectDuration = boolValue(SEC_OPTION_FORCE_CORRECT);
        mAutoPlay = boolValue(SEC_OPTION_AUTO_PLAY);
        mMultiInstance = boolValue(SEC_OPTION_MULTI_INSTANCE);
        mDisplayTrayIcon = boolValue(SEC_OPTION_DISPLAY_TRAY_ICON, true);
        mPilotDuration = intValue(SEC_OPTION_PILOT_DURATION, 150000);
        mCheckDuration = intValue(SEC_OPTION_CHECK_DURATION, 3000);
        mExportProcesses = intValue(SEC_OPTION_EXPORT_PROCESSES, 2);
        mAutoClearExportMissions = boolValue(SEC_OPTION_EXPORT_AUTO_CLEAR);
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

void TPreferences::save()
{
    mSettings->sync();
}

QString TPreferences::language()
{
    return mLanguage;
}

void TPreferences::setLanguage(QString language)
{
    SET_VALUE(language, mLanguage, SEC_GUI, SEC_GUI_LANGUAGE);

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
    SET_VALUE(path, mLastOpenPath, SEC_GUI, SEC_GUI_LAST_OPEN_PATH);
}

void TPreferences::setLastOpenDirectory(QString path)
{
    SET_VALUE(path, mLastOpenDir, SEC_GUI, SEC_GUI_LAST_OPEN_DIR);
}

bool TPreferences::autoCorrectDuration()
{
    return mAutoCorrectDuration;
}

void TPreferences::setCorrectDuration(bool autoCorrect)
{
    SET_VALUE2(autoCorrect, mAutoCorrectDuration, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_AUTO_CORRECT);
}

bool TPreferences::forceCorrectDuration()
{
    return mForceCorrectDuration;
}

void TPreferences::setForceCorrectDuration(bool force)
{
    SET_VALUE2(force, mForceCorrectDuration, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_FORCE_CORRECT);
}

int TPreferences::pilotDuration()
{
    return mPilotDuration;
}

void TPreferences::setPilotDuration(int duration)
{
    SET_VALUE2(duration, mPilotDuration, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_PILOT_DURATION);
}

int TPreferences::checkDuration()
{
    return mCheckDuration;
}

void TPreferences::setCheckDuration(int duration)
{
    SET_VALUE2(duration, mCheckDuration, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_CHECK_DURATION);
}

bool TPreferences::autoPlayAfterStarted()
{
    return mAutoPlay;
}

void TPreferences::setAutoPlayAfterStarted(bool bEnabled)
{
    SET_VALUE2(bEnabled, mAutoPlay, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_AUTO_PLAY);
}

bool TPreferences::displayTrayIcon()
{
    return mDisplayTrayIcon;
}

void TPreferences::setDisplayTrayIcon(bool bEnabled)
{
    SET_VALUE2(bEnabled, mDisplayTrayIcon, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_DISPLAY_TRAY_ICON);
}

bool TPreferences::enableMultiInstance()
{
    return mMultiInstance;
}

void TPreferences::setEnableMultiInstance(bool bEnabled)
{
    SET_VALUE2(bEnabled, mMultiInstance, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_MULTI_INSTANCE);
}

int TPreferences::exportProcesses()
{
    return mExportProcesses;
}

void TPreferences::setExportProcesses(int value)
{
    SET_VALUE2(value, mExportProcesses, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_EXPORT_PROCESSES);
}

bool TPreferences::autoClearExportMissions()
{
    return mAutoClearExportMissions;
}

void TPreferences::setAutoClearExportMissions(bool value)
{
    SET_VALUE2(value, mAutoClearExportMissions, SEC_OPTIONS, SEC_OPTION_GENERAL, SEC_OPTION_EXPORT_AUTO_CLEAR);
}

QString TPreferences::skinPath()
{
    return mSkinPath;
}

void TPreferences::setSkinPath(QString skinName)
{
    if(mSkinPath==skinName)
        return;

    mSettings->beginGroup(SEC_GUI);
    mSettings->setValue(SEC_GUI_LAST_SKIN_PATH, skinName);
    mSettings->endGroup();

    mSkinPath = skinName;
}

bool TPreferences::muteEnabled()
{
    return mMuteEnabled;
}

void TPreferences::setMuteEnabled(bool enabled)
{
    if(mMuteEnabled==enabled)
        return;

    mSettings->beginGroup(SEC_MAIN_WINDOW);
    mSettings->setValue(SEC_MAIN_WINDOW_MUTE, enabled);
    mSettings->endGroup();

    mMuteEnabled = enabled;
}

float TPreferences::opacity()
{
    return mOpacity;
}

void TPreferences::setOpacity(float value)
{
    SET_VALUE(value, mOpacity, SEC_GUI, SEC_GUI_OPACITY);
}

PlayMode TPreferences::playMode()
{
    return mPlayMode;
}

void TPreferences::setPlayMode(PlayMode playMode)
{
    SET_VALUE(playMode, mPlayMode, SEC_MAIN_WINDOW, SEC_MAIN_WINDOW_PLAY_MODE);
}

int TPreferences::volumeValue()
{
    return mVolumeValue;
}

void TPreferences::setVolumeValue(int value)
{
    SET_VALUE(value, mVolumeValue, SEC_MAIN_WINDOW, SEC_MAIN_WINDOW_VOLUME);
}

bool TPreferences::lyricWindowVisible()
{
    return mLyricWindowVisible;
}

void TPreferences::setLyricWindowVisible(bool bVisible)
{
    SET_VALUE(bVisible, mLyricWindowVisible, SEC_MAIN_WINDOW, SEC_MAIN_WINDOW_LYRIC_BTN);
}

bool TPreferences::eqWindowVisible()
{
    return mEqWindowVisible;
}

void TPreferences::setEqWindowVisible(bool bVisible)
{
    SET_VALUE(bVisible, mEqWindowVisible, SEC_MAIN_WINDOW, SEC_MAIN_WINDOW_EQ_BTN);
}

bool TPreferences::playlistWindowVisible()
{
    return mPlaylistWindowVisible;
}

void TPreferences::setPlaylistWindowVisible(bool bVisible)
{
    SET_VALUE(bVisible, mPlaylistWindowVisible, SEC_MAIN_WINDOW, SEC_MAIN_WINDOW_PLAYLIST_BTN);
}

bool TPreferences::eqEnabled()
{
    return mEqEnabled;
}

void TPreferences::setEqEnabled(bool bEnabled)
{
    SET_VALUE(bEnabled, mEqEnabled, SEC_EQUALIZER_WINDOW, SEC_EQUALIZER_WINDOW_ENABLED);
}

int TPreferences::eqBallance()
{
    return mEqBallance;
}

void TPreferences::setEqBallance(int value)
{
    SET_VALUE(value, mEqBallance, SEC_EQUALIZER_WINDOW, SEC_EQUALIZER_WINDOW_BALLANCE);
}

int TPreferences::eqSurround()
{
    return mEqSurround;
}

void TPreferences::setEqSurround(int value)
{
    SET_VALUE(value, mEqSurround, SEC_EQUALIZER_WINDOW, SEC_EQUALIZER_WINDOW_SURROUND);
}

int TPreferences::eqAmplification()
{
    return mEqAmplification;
}

void TPreferences::setEqAmplification(int value)
{
    SET_VALUE(value, mEqAmplification, SEC_EQUALIZER_WINDOW, SEC_EQUALIZER_WINDOW_AMPLIFY);
}

QList<int> TPreferences::eqFactors()
{
    return mEqFactors;
}

void TPreferences::setEqFactors(QList<int> value)
{
    if(mEqFactors==value)
        return;

    QStringList sl;
    for(int factor : value)
    {
        sl.append(QString::number(factor));
    }
    mSettings->beginGroup(SEC_EQUALIZER_WINDOW);
    mSettings->setValue(SEC_EQUALIZER_WINDOW_FACTORS, sl.join(','));
    mSettings->endGroup();

    mEqFactors = value;
}

int TPreferences::eqProfile()
{
    return mEqProfile;
}

void TPreferences::setEqProfile(int profile)
{
    SET_VALUE(profile, mEqProfile, SEC_EQUALIZER_WINDOW, SEC_EQUALIZER_WINDOW_PROFILE);
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
    for(QVariant v : list) {
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
