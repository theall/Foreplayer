#include "tplaylistwindow.h"

TPlaylistWindow::TPlaylistWindow(QWidget *parent) :
    TAbstractWindow(parent),
    mToolbar(new TToolBar(this)),
    mBtnClose(new TImageButton(this)),
    mCentralWidget(new TPlaylistWidget(this))
{
    setObjectName("PlaylistWindow");

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));

    setMouseTracking(true);

    retranslateUi();
}

void TPlaylistWindow::setGuiParamter(PlaylistWindowParam *param)
{
    setWindowParam(&param->window);

    mBtnClose->setPixmapRect(param->close.image, param->close.position.geometry());
    mBtnClose->setAlignment(param->window.background, param->close.alignment);

    mToolbar->setPixmaps(param->toolbar.image, param->toolbar.image1);
    mToolbar->setGeometry(param->toolbar.position.geometry());

    mToolbar->setAlignment(param->window.background, param->toolbar.alignment);
}

void TPlaylistWindow::on_btnClose_clicked()
{
    hide();

    emit playlistWindowToggled(false);
}

void TPlaylistWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mToolbar->retranslateUi();
}

void TPlaylistWindow::resizeEvent(QResizeEvent *event)
{
    TAbstractWindow::resizeEvent(event);

    mBtnClose->updatePos();
    mToolbar->updatePos();
    QRect rt = rect();
    mCentralWidget->setGeometry(rt);
}
