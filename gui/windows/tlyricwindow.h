#ifndef TLYRICWINDOW_H
#define TLYRICWINDOW_H

#include "pch.h"
#include "../skinutils.h"

#include "widgets/timagebutton.h"
#include "tabstractwindow.h"

class TLyricWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TLyricWindow(QWidget *parent = 0);

    void setGuiParamter(LyricWindowParam *param);

signals:
    void requireShowDesktopLyric();
    void lyricWindowToggled(bool);
    void lyricWindowOntopToggled(bool);

public slots:

private slots:
    void on_btnClose_clicked();
    void on_btnOnTop_clicked(bool checked);
    void on_btnShowDescktopLyric_clicked();

private:
    QWidget *mLyric;
    TImageButton *mBtnClose;
    TImageButton *mBtnOnTop;
    TImageButton *mBtnDesktopLyric;

    // QWidget interface
protected:
    void retranslateUi() Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
};





#endif // TLYRICWINDOW_H
