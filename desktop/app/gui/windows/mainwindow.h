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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/label.h"
#include "widgets/imagebutton.h"
#include "widgets/sliderbar.h"
#include "widgets/visualwidget.h"
#include "widgets/scrolllabel.h"
#include "widgets/ledwidget.h"
#include "abstractwindow.h"

class TMainWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TMainWindow(QWidget *parent = 0);
    ~TMainWindow();

    void setProgress(int time, int total);
    void setTitles(QStringList titles);
    void setTitles(QString title);
    void setCaption(QString title);
    void setPlayState(QString state);
    void setEffect(QString effect);

    void checkLyricButton(bool checked);
    void checkEqualizerButton(bool checked);
    void checkPlaylistButton(bool checked);
    void checkBrowserButton(bool checked);

    void setContextMenu(QMenu *menu);

    void setButtonPlayChecked(bool checked);

    const TScrollLabel *informationLable() const
    {
        return mMusicTitle;
    }
    TVisualWidget *visualWidget() const
    {
        return mVisualWidget;
    }
    const TLedWidget *ledWidget() const
    {
        return mLedTime;
    }

signals:
    // For controller
    void playClicked();
    void pauseClicked();
    void prevClicked();
    void nextClicked();
    void stopClicked();

    // For gui control
    void exitClicked();
    void miniModeClicked();
    void equalizerButtonToggle(bool);
    void lyricButtonToggle(bool);
    void playlistButtonToggle(bool);
    void browserButtonToggle(bool);
    void volumeToggle(bool);
    void requestSwitchToMiniMode();

    // Volume between 0.0~1.0f
    void volumeValueChanged(float);
    void progressChanged(int);
    void requestOpenFiles(QStringList);
    void requestShowMinimized();
    void onActivationChange();

private slots:
    void on_btnPlay_clicked();
    void on_btnPause_clicked();
    void on_btnOpen_clicked();
    void on_volume_valueChanged(int value);
    void on_btnMute_clicked();
    void on_icnLogo_clicked();

private:
    TImageButton *mBtnPlay;
    TImageButton *mBtnPrev;
    TImageButton *mBtnNext;
    TImageButton *mBtnPause;
    TImageButton *mBtnStop;
    TImageButton *mBtnOpen;
    TImageButton *mBtnMute;
    TImageButton *mBtnMinimize;
    TImageButton *mBtnBrowser;
    TImageButton *mBtnEqualizer;
    TImageButton *mBtnPlaylist;
    TImageButton *mBtnLyrics;
    TImageButton *mBtnExit;
    TImageButton *mBtnMinimode;
    TSliderBar *mProgressBar;
    TSliderBar *mVolumeBar;
    TLabel *mIcon;
    TScrollLabel *mMusicTitle;
    TLedWidget *mLedTime;
    TLabel *mStereo;
    TLabel *mStatus;
    TVisualWidget* mVisualWidget;
    QString mCaption;
    int mCaptionIndex;
    int mTitleTimerId;
    bool mMinimode;
    bool mWindowHided;
    QMenu *mContextMenu;
    QString mPlayState;
    QString mPlayEffect;
    QIcon mDefaultLogo;

    void updatePlayStatus();
    void updatePlayEffect();

protected:
    void retranslateUi() Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;

    // TXmlRead interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
    void changeEvent(QEvent *) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
