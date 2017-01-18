#include "desctoplyricwindow.h"

TDesktopLyricWindow::TDesktopLyricWindow(QWidget *parent) : TAbstractWindow(parent, true)
{
    setObjectName("DesktopLyricWindow");

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
    setWindowTitle(tr("DesklrcBar"));
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

void TDesktopLyricWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnIcon->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_ICON), skin);
    mBtnPlay->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_PLAY), skin);
    mBtnPause->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_PAUSE), skin);
    mBtnPrev->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_PREV), skin);
    mBtnNext->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_NEXT), skin);
    mBtnList->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_LIST), skin);
    mBtnSettings->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_SETTINGS), skin);
    mBtnKalaok->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_KALAOK), skin);
    mBtnLines->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_LINES), skin);
    mBtnLock->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_LOCK), skin);
    mBtnOnTop->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_ONTOP), skin);
    mBtnBack->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_RETURN), skin);
    mBtnClose->loadFromSkin(element.firstChildElement(TAG_DESKTOP_LYRIC_CLOSE), skin);
}
