#include "playlistwindow.h"

TPlaylistWindow::TPlaylistWindow(QWidget *parent) :
    TAbstractWindow(parent)
{
    setObjectName("PlaylistWindow");

    setMouseTracking(true);

    TPlaylistWidget::setFontColors(QFont("-11,0,0,0,400,0,0,0,0,3,2,4,34,Tahoma"),
                  QColor("#6091c6"),
                  QColor("#e3ecf4"),
                  QColor("#aac5dd"),
                  QColor("#6898c4"),
                  QColor("#3c80c4"),
                  QColor("#000000"),
                  QColor("#0c1623"));

    TPlaylistWidget::setScrollBarPixmaps(
                QPixmap("z:/skins/fulkfour/b1.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_button.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_thumb.bmp")
                );

    mToolbar = new TToolBar(this);
    mBtnClose = new TImageButton(this);
    mCentralWidget = new TPlaylistWidget(this);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));

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

    mCentralWidget->setAlignment(param->window.background, param->playlist.position.geometry());
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
    if(mCentralWidget->updatePosition(event->size()))
    {
        TAbstractWindow::resizeEvent(event);

        mBtnClose->updatePos();
        mToolbar->updatePos();
    }
}
