#include "lyricwindow.h"

TLyricWindow::TLyricWindow(QWidget *parent) : TAbstractWindow(parent)
{
    setObjectName("LyricWindow");

    mBtnClose = new TImageButton(this);
    mBtnOnTop = new TImageButton(this);
    mBtnDesktopLyric = new TImageButton(this);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mBtnOnTop, SIGNAL(clicked(bool)), this, SLOT(on_btnOnTop_clicked(bool)));
    connect(mBtnDesktopLyric, SIGNAL(clicked()), this, SLOT(on_btnShowDescktopLyric_clicked()));

    setMouseTracking(true);

    retranslateUi();
}

void TLyricWindow::setGuiParamter(LyricWindowParam *param)
{
    setWindowParam(&param->window);

    mBtnClose->setPixmapRect(param->close.image, param->close.position.geometry());
    mBtnOnTop->setPixmapRect(param->ontop.image, param->ontop.position.geometry());
    mBtnDesktopLyric->setPixmapRect(param->desklyc.image, param->desklyc.position.geometry());

    mBtnClose->setAlignment(param->window.background, param->close.alignment);
    mBtnOnTop->setAlignment(param->window.background, param->ontop.alignment);
    mBtnDesktopLyric->setAlignment(param->window.background, param->desklyc.alignment);
}

void TLyricWindow::on_btnClose_clicked()
{
    hide();

    emit lyricWindowToggled(false);
}

void TLyricWindow::on_btnShowDescktopLyric_clicked()
{
    hide();
    emit requireShowDesktopLyric();
}

void TLyricWindow::on_btnOnTop_clicked(bool checked)
{
    Qt::WindowFlags flags = windowFlags();
    if(checked)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags ^= Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    emit lyricWindowOntopToggled(checked);
}

void TLyricWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mBtnOnTop->setToolTip(tr("Always on top"));
    mBtnDesktopLyric->setToolTip(tr("Show desctop lyric"));
}

void TLyricWindow::resizeEvent(QResizeEvent *event)
{
    TAbstractWindow::resizeEvent(event);

    mBtnClose->updatePos();
    mBtnOnTop->updatePos();
    mBtnDesktopLyric->updatePos();
}
