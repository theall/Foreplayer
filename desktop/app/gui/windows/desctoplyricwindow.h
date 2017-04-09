#ifndef TDESKTOPLYRICWINDOW_H
#define TDESKTOPLYRICWINDOW_H

#include "widgets/imagebutton.h"
#include "abstractwindow.h"

class TDesktopLyricWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TDesktopLyricWindow(QWidget *parent = 0);

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

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TDESKTOPLYRICWINDOW_H
