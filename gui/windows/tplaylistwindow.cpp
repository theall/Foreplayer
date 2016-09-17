#include "tplaylistwindow.h"

#include "tsplitter.h"

TPlaylistWindow::TPlaylistWindow(QWidget *parent) :
    TAbstractWindow(parent),
    mToolbar(new TToolBar(this)),
    mBtnClose(new TImageButton(this))
{
    setObjectName("PlaylistWindow");

    QVBoxLayout *layout = new QVBoxLayout(this);
    TSplitter *splitter = new TSplitter(this);
    splitter->addWidget(new QListView(this));
    splitter->addWidget(new QTableView(this));

    layout->addWidget(mBtnClose);
    layout->addWidget(splitter);
    layout->addWidget(mToolbar);

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
}
