#ifndef TDESKTOPLYRICWINDOW_H
#define TDESKTOPLYRICWINDOW_H

#include "pch.h"
#include "../skinutils.h"

#include "widgets/imagebutton.h"
#include "abstractwindow.h"

class TDesktopLyricWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TDesktopLyricWindow(QWidget *parent = 0);

    void setGuiParamter(DesklrcBarParam *param);

signals:
    void requireShowLyricWindow();
    void lyricWindowToggled(bool);
    void lyricWindowOntopToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();
    void on_btnOnTop_clicked(bool checked);
    void on_btnBack_clicked();

private:
    TImageButton *mBtnDesktopLyric;
    TImageButton *mBtnIcon;
    TImageButton *mBtnPlay;
    TImageButton *mBtnPause;
    TImageButton *mBtnPrev;
    TImageButton *mBtnNext;
    TImageButton *mBtnList;
    TImageButton *mBtnSettings;
    TImageButton *mBtnKalaok;
    TImageButton *mBtnLines;
    TImageButton *mBtnLock;
    TImageButton *mBtnOnTop;
    TImageButton *mBtnBack;
    TImageButton *mBtnClose;
};

#endif // TDESKTOPLYRICWINDOW_H
