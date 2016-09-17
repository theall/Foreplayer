#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "skinutils.h"
#include "tlabel.h"
#include "timagebutton.h"
#include "tsliderbar.h"
#include "tvisualwidget.h"
#include "tscrolllabel.h"
#include "tledwidget.h"
#include "tabstractwindow.h"

class TMainWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TMainWindow(QWidget *parent = 0);
    ~TMainWindow();

    void setGuiParameter(PlayerWindowParam *param, PlayerWindowParam *miniParam);

    void setTime(int time, int total);
    void setTitles(QStringList titles);
    void setCaption(QString title);
    void setPlayState(int state);
    void setEffect(bool);

    void checkLyricButton(bool checked);
    void checkEqualizerButton(bool checked);
    void checkPlaylistButton(bool checked);
    void checkBrowserButton(bool checked);

    void setContextMenu(QMenu *menu);

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
    void playClicked();
    void pauseClicked();
    void prevClicked();
    void nextClicked();
    void stopClicked();
    void exitClicked();
    void equalizerButtonToggle(bool);
    void lyricButtonToggle(bool);
    void playlistButtonToggle(bool);
    void browserButtonToggle(bool);
    void volumeToggle(bool);
    void showModeSwitch(bool normal);
    void volumeValueChanged(int);
    void progressChanged(int);
    void requestOpenFiles(QStringList);
    void requestShowMinimized();
    void requestToggleWindow();
    void requestRestoreWindow();

private slots:
    void on_btnPlay_clicked();
    void on_btnPause_clicked();
    void on_btnOpen_clicked();
    void on_volume_valueChanged(int value);
    void on_minimode_Clicked();
    void slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

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
    PlayerWindowParam mMainParam;
    PlayerWindowParam mMiniParam;
    QSystemTrayIcon *mTrayIcon;
    bool mMinimode;
    bool mWindowHided;
    QMenu *mContextMenu;
    void updateGuiParameter();
    void updateMinimodeTooltip();

protected:
    void retranslateUi() Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
