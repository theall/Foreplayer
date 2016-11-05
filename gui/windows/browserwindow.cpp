#include "browserwindow.h"

TBrowserWindow::TBrowserWindow(QWidget *parent) : TAbstractWindow(parent)
{
    setObjectName("BrowserWindow");

    mBtnClose = new TImageButton(this);
    mBtnBackward = new TImageButton(this);
    mBtnForward = new TImageButton(this);
    mBtnRefresh = new TImageButton(this);
    mCkbStartup = new TCheckBox(this);
    mLabelLink = new TLinkLabel(this);
    mBtnBrowser = new TImageButton(this);
    mBtnMini = new TImageButton(this);
    mBtnDownload = new TImageButton(this);

    mCkbStartup->setCheckable(true);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));

    retranslateUi();
}

void TBrowserWindow::setGuiParamter(BrowserWindowParam *param)
{
    setWindowParam(&param->window);

    mBtnClose->setPixmapRect(param->close.image, param->close.position.geometry());
    mBtnBackward->setPixmapRect(param->backward.image, param->backward.position.geometry());
    mBtnForward->setPixmapRect(param->forward.image, param->forward.position.geometry());
    mBtnRefresh->setPixmapRect(param->refresh.image, param->refresh.position.geometry());

    mCkbStartup->setPixmapRect(param->startup.image, param->startup.position.geometry());
    mCkbStartup->setFontColor(param->startup.font, param->startup.color);

    mLabelLink->setFontColor(param->linktxt.font, param->linktxt.color);
    mBtnBrowser->setPixmapRect(param->browser.image, param->browser.position.geometry());
    mBtnMini->setPixmapRect(param->minibtn.image, param->minibtn.position.geometry());
    mBtnDownload->setPixmapRect(param->downloadbtn.image, param->downloadbtn.position.geometry());
}

void TBrowserWindow::on_btnClose_clicked()
{
    hide();

    emit browserWindowToggled(false);
}

void TBrowserWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mBtnBackward->setToolTip(tr("Backward"));
    mBtnForward->setToolTip(tr("Forward"));
    mBtnRefresh->setToolTip(tr("Refresh"));
    mCkbStartup->setToolTip(tr("Diasplay on startup"));
    mLabelLink->setText(tr("Use instructions"));
    mBtnBrowser->setToolTip(tr("Browser"));
    mBtnMini->setToolTip(tr("Mini"));
    mBtnDownload->setToolTip(tr("Download"));
}
