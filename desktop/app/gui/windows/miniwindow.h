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
#ifndef TMINIWINDOW_H
#define TMINIWINDOW_H

#include "widgets/label.h"
#include "widgets/imagebutton.h"
#include "widgets/sliderbar.h"
#include "widgets/visualwidget.h"
#include "widgets/scrolllabel.h"
#include "widgets/ledwidget.h"
#include "abstractwindow.h"

class TMiniWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TMiniWindow(QWidget *parent = 0);
    ~TMiniWindow();

    void setContextMenu(QMenu *menu);

signals:
    void playClicked();
    void pauseClicked();
    void prevClicked();
    void nextClicked();
    void stopClicked();
    void exitClicked();
    void lyricButtonToggle(bool);
    void volumeToggle(bool);
    void volumeValueChanged(int);
    void progressChanged(int);
    void requestShowMinimized();
    void openMusicsClicked();
    void normalModeClicked();
    void iconClicked();

private:
    TImageButton *mBtnPlay;
    TImageButton *mBtnPrev;
    TImageButton *mBtnNext;
    TImageButton *mBtnPause;
    TImageButton *mBtnStop;
    TImageButton *mBtnOpen;
    TImageButton *mBtnMute;
    TImageButton *mBtnMinimize;
    TImageButton *mBtnLyrics;
    TImageButton *mBtnExit;
    TImageButton *mBtnNormalMode;
    TSliderBar *mProgressBar;
    TSliderBar *mVolumeBar;
    TLabel *mIcon;
    TScrollLabel *mMusicTitle;
    TLedWidget *mLedTime;
    TLabel *mStereo;
    TLabel *mStatus;
    TVisualWidget* mVisualWidget;

    QMenu *mContextMenu;

private slots:
    void on_icoLogo_clicked();

    // TAbstractWindow interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TMINIWINDOW_H
