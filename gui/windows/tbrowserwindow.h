#ifndef TBROWSERWINDOW_H
#define TBROWSERWINDOW_H

#include "pch.h"
#include "skinutils.h"
#include "tlinklabel.h"
#include "tcheckbox.h"
#include "timagebutton.h"
#include "tabstractwindow.h"

class TBrowserWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TBrowserWindow(QWidget *parent = 0);

    void setGuiParamter(BrowserWindowParam *param);

signals:
    void browserWindowToggled(bool);

public slots:

protected:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void on_btnClose_clicked();

private:
    TImageButton *mBtnClose;
    TImageButton *mBtnBackward;
    TImageButton *mBtnForward;
    TImageButton *mBtnRefresh;
    TImageButton *mBtnBrowser;
    TImageButton *mBtnMini;
    TImageButton *mBtnDownload;
    TCheckBox *mCkbStartup;
    TLinkLabel *mLabelLink;
};

#endif // TBROWSERWINDOW_H
