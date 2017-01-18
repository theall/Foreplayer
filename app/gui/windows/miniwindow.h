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
    void requestSwitchToMiniMode(bool normal);
    void volumeValueChanged(int);
    void progressChanged(int);
    void requestOpenFiles(QStringList);
    void requestShowMinimized();
    void requestToggleWindow();

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

    // TAbstractWindow interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TMINIWINDOW_H
