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
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "pch.h"

enum TWindowType
{
    WT_MAIN,
    WT_LYRIC,
    WT_EQUALIZER,
    WT_PLAYLIST,
    WT_BROWSER,
    WindowTypeCount
};

enum PlayMode
{
    PM_MANUAL,
    PM_RECYCLE_TRACK,
    PM_RECYCLE_TRACK_LIST,
    PM_RECYCLE_PLAY_LIST,
    PM_RECYCLE_ALL,
    PM_RANDOM
};

class TPreferences : public QObject
{
    Q_OBJECT

public:
    TPreferences(QObject *parent=0);

    ~TPreferences();

    static TPreferences *instance();
    static void deleteInstance();

    void save();

    QString language();
    void setLanguage(QString language);
    int runCount();
    void windowGeometryState(TWindowType type, QByteArray* g, QByteArray* s);
    void setWindowGeometryState(TWindowType type, QVariant geometry, QVariant windowState);
    QString lastOpenDialogPath();
    QString lastOpenDirectory();
    void setLastOpenDialogPath(QString path);
    void setLastOpenDirectory(QString path);

    bool autoCorrectDuration();
    void setCorrectDuration(bool autoCorrect);

    bool forceCorrectDuration();
    void setForceCorrectDuration(bool force);

    int pilotDuration();
    void setPilotDuration(int duration);

    int checkDuration();
    void setCheckDuration(int duration);

    bool autoPlayAfterStarted();
    void setAutoPlayAfterStarted(bool bEnabled);

    bool displayTrayIcon();
    void setDisplayTrayIcon(bool bEnabled);

    bool enableMultiInstance();
    void setEnableMultiInstance(bool bEnabled);

    int exportProcesses();
    void setExportProcesses(int value);

    bool autoClearExportMissions();
    void setAutoClearExportMissions(bool value);

    QString skinPath();
    void setSkinPath(QString skinName);

    bool muteEnabled();
    void setMuteEnabled(bool enabled);

    float opacity();
    void setOpacity(float value);

    PlayMode playMode();
    void setPlayMode(PlayMode playMode);

    int volumeValue();
    void setVolumeValue(int value);

    bool lyricWindowVisible();
    void setLyricWindowVisible(bool bVisible);
    bool eqWindowVisible();
    void setEqWindowVisible(bool bVisible);
    bool playlistWindowVisible();
    void setPlaylistWindowVisible(bool bVisible);

    bool eqEnabled();
    void setEqEnabled(bool bEnabled);
    int eqBallance();
    void setEqBallance(int value);
    int eqSurround();
    void setEqSurround(int value);
    int eqAmplification();
    void setEqAmplification(int value);
    QList<int> eqFactors();
    void setEqFactors(QList<int> value);
    int eqProfile();
    void setEqProfile(int profile);

signals:
    void languageChanged();

private:
    QSettings *mSettings;
    bool mAlwaysTop;
    int mRunCount;
    QString mLanguage;
    QString mLastOpenPath;
    QString mLastOpenDir;
    QString mSkinPath;
    bool mAutoCorrectDuration;
    bool mForceCorrectDuration;
    bool mAutoPlay;
    bool mMultiInstance;
    bool mDisplayTrayIcon;
    int mPilotDuration;
    int mCheckDuration;
    bool mMuteEnabled;
    int mVolumeValue;
    bool mLyricWindowVisible;
    bool mEqWindowVisible;
    bool mPlaylistWindowVisible;
    bool mEqEnabled;
    int mEqBallance;
    int mEqSurround;
    int mEqAmplification;
    int mEqProfile;
    int mExportProcesses;
    QList<int> mEqFactors;
    float mOpacity;
    PlayMode mPlayMode;
    bool mAutoClearExportMissions;

    void setValue(QString section, QVariant value);
    QVariant value(QString section, QVariant defValue=QVariant());
    bool boolValue(QString key, bool defValue = false);
    QColor colorValue(QString key, QColor defValue = QColor());
    QString stringValue(QString key, QString defValue = "");
    int intValue(QString key, int defaultValue=0);
    float floatValue(QString key, float defaultValue=0.0);
    QDate dateValue(QString key, int defaultValue=-1);
    QTime timeValue(QString key, int defaultValue=0);
    QStringList listValue(QString key);
    static TPreferences* mInstance;
};

#endif // PREFERENCES_H
