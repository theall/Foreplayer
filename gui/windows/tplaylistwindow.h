#ifndef TPLAYLISTWINDOW_H
#define TPLAYLISTWINDOW_H

#include "pch.h"
#include "skinutils.h"
#include "timagebutton.h"
#include "ttoolbar.h"
#include "tabstractwindow.h"

class TPlaylistWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TPlaylistWindow(QWidget *parent = 0);

    void setGuiParamter(PlaylistWindowParam *param);

signals:
    void playlistWindowToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();

private:
    TToolBar *mToolbar;
    TImageButton *mBtnClose;
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;
};



#endif // TPLAYLISTWINDOW_H
