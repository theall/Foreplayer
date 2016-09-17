#include "tdesctoplyricwindow.h"

TDesktopLyricWindow::TDesktopLyricWindow(QWidget *parent) : TAbstractWindow(parent)
{
    setObjectName("DesktopLyricWindow");

    mBtnDesktopLyric = new TImageButton(this);
    mBtnIcon = new TImageButton(this);
    mBtnPlay = new TImageButton(this);
    mBtnPause = new TImageButton(this);
    mBtnPrev = new TImageButton(this);
    mBtnNext = new TImageButton(this);
    mBtnList = new TImageButton(this);
    mBtnSettings = new TImageButton(this);
    mBtnKalaok = new TImageButton(this);
    mBtnLines = new TImageButton(this);
    mBtnLock = new TImageButton(this);
    mBtnOnTop = new TImageButton(this);
    mBtnBack = new TImageButton(this);
    mBtnClose = new TImageButton(this);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mBtnOnTop, SIGNAL(clicked(bool)), this, SLOT(on_btnOnTop_clicked(bool)));
    connect(mBtnBack, SIGNAL(clicked()), this, SLOT(on_btnBack_clicked()));

    retranslateUi();
}

void TDesktopLyricWindow::setGuiParamter(DesklrcBarParam *param)
{
    setWindowParam(&param->window);

    mBtnDesktopLyric->setPixmapRect(param->desklrc_bar.image, param->desklrc_bar.position.geometry());
    mBtnIcon->setPixmapRect(param->icon.image, param->icon.position.geometry());
    mBtnPlay->setPixmapRect(param->play.image, param->play.position.geometry());
    mBtnPause->setPixmapRect(param->pause.image, param->pause.position.geometry());
    mBtnPrev->setPixmapRect(param->prev.image, param->prev.position.geometry());
    mBtnNext->setPixmapRect(param->next.image, param->next.position.geometry());
    mBtnList->setPixmapRect(param->list.image, param->list.position.geometry());
    mBtnSettings->setPixmapRect(param->settings.image, param->settings.position.geometry());
    mBtnKalaok->setPixmapRect(param->kalaok.image, param->kalaok.position.geometry());
    mBtnLines->setPixmapRect(param->lines.image, param->lines.position.geometry());
    mBtnLock->setPixmapRect(param->lock.image, param->lock.position.geometry());
    mBtnOnTop->setPixmapRect(param->ontop.image, param->ontop.position.geometry());
    mBtnBack->setPixmapRect(param->back.image, param->back.position.geometry());
    mBtnClose->setPixmapRect(param->close.image, param->close.position.geometry());
}

void TDesktopLyricWindow::on_btnClose_clicked()
{
    hide();

    emit lyricWindowToggled(false);
}

void TDesktopLyricWindow::on_btnBack_clicked()
{
    hide();
    emit requireShowLyricWindow();
}

void TDesktopLyricWindow::on_btnOnTop_clicked(bool checked)
{
    Qt::WindowFlags flags = windowFlags();
    if(checked)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags ^= Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    emit lyricWindowOntopToggled(checked);
}

void TDesktopLyricWindow::retranslateUi()
{
    mBtnDesktopLyric->setToolTip(tr("DesklrcBar"));
    mBtnIcon->setToolTip(tr("Main menu"));
    mBtnPlay->setToolTip(tr("Play"));
    mBtnPause->setToolTip(tr("Pause"));
    mBtnPrev->setToolTip(tr("Previous"));
    mBtnNext->setToolTip(tr("Next"));
    mBtnList->setToolTip(tr("Choose music"));
    mBtnSettings->setToolTip(tr("Settings"));
    mBtnKalaok->setToolTip(tr("Kalaok"));
    mBtnLines->setToolTip(tr("Double line"));
    mBtnLock->setToolTip(tr("Lock"));
    mBtnOnTop->setToolTip(tr("Ontop"));
    mBtnBack->setToolTip(tr("Back"));
    mBtnClose->setToolTip(tr("Close"));
}
