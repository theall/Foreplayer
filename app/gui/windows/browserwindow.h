#ifndef TBROWSERWINDOW_H
#define TBROWSERWINDOW_H

#include "pch.h"

#include "widgets/linklabel.h"
#include "widgets/checkbox.h"
#include "widgets/imagebutton.h"
#include "abstractwindow.h"

class TBrowserWindow : public TAbstractWindow
{
    Q_OBJECT
public:
    explicit TBrowserWindow(QWidget *parent = 0);

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

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TBROWSERWINDOW_H
