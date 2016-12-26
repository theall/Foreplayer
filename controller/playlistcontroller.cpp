#include "playlistcontroller.h"

TPlaylistController::TPlaylistController(QObject *parent) :
    TAbstractController(parent),
    mPlaylistModel(new TPlaylistModel(this)),
    mMusiclistModel(new TMusiclistModel(this)),
    mTracklistModel(new TTrackListModel(this)),
    mPlaylistWidget(NULL),
    mPlaylistCore(NULL)
{

}

void TPlaylistController::joint(TGuiManager *gui, TCore *core)
{
    Q_ASSERT(gui);
    Q_ASSERT(core);

    TAbstractModel::setColorParameters(QFont("-11,0,0,0,400,0,0,0,0,3,2,4,34,Tahoma"),
                  QColor("#6091c6"),
                  QColor("#e3ecf4"),
                  QColor("#aac5dd"),
                  QColor("#6898c4"),
                  QColor("#3c80c4"),
                  QColor("#000000"));

    TPlaylistWidget::setFontColors(QColor("#0c1623"));
    TScrollBar::setPixmaps(
                QPixmap("z:/skins/fulkfour/b1.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_button.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_thumb.bmp")
                );

    mPlaylistWidget = gui->playlistWindow()->playlistWidget();

    Q_ASSERT(mPlaylistWidget);

    connect(mPlaylistWidget, SIGNAL(onPlaylistIndexChanged(int)), this, SLOT(slotPlaylistIndexChanged(int)));
    connect(mPlaylistWidget, SIGNAL(onMusiclistIndexChanged(int)), this, SLOT(slotMusiclistIndexChanged(int)));
    connect(mPlaylistWidget, SIGNAL(onTracklistIndexChanged(int)), this, SLOT(slotTracklistIndexChanged(int)));
//    connect(mPlaylistWidget,
//            SIGNAL(requestSetColors(QFont,QColor,QColor,QColor,QColor,QColor,QColor)),
//            this,
//            SLOT(slotRequestSetColors(QFont,QColor,QColor,QColor,QColor,QColor,QColor)));

    mPlaylistCore = core->playlist();
    mPlaylistModel->setPlaylistCore(mPlaylistCore);
    mMusiclistModel->setPlaylistCore(mPlaylistCore);
    mTracklistModel->setPlaylistCore(mPlaylistCore);

    mPlaylistWidget->playlistView()->setModel(mPlaylistModel);
    mPlaylistWidget->musiclistView()->setModel(mMusiclistModel);
    mPlaylistWidget->tracklistView()->setModel(mTracklistModel);

    TAbstractController::joint(gui, core);
}

void TPlaylistController::slotPlaylistIndexChanged(int index)
{
    if(mPlaylistModel)
    {
        mPlaylistModel->setCurrentIndex(index);
    }
}

void TPlaylistController::slotMusiclistIndexChanged(int index)
{
    if(mMusiclistModel)
    {
        mMusiclistModel->setCurrentIndex(index);
    }
}

void TPlaylistController::slotTracklistIndexChanged(int index)
{
    if(mTracklistModel)
    {
        mTracklistModel->setCurrentIndex(index);
    }
}

void TPlaylistController::slotRequestSetColors(
        QFont font,
        QColor textColor,
        QColor selectedTextColor,
        QColor numberColor,
        QColor durationColor,
        QColor currentRowTextColor,
        QColor backgroundColor)
{
    TAbstractModel::setColorParameters(font,
                                       textColor,
                                       selectedTextColor,
                                       numberColor,
                                       durationColor,
                                       currentRowTextColor,
                                       backgroundColor);
}

void TPlaylistController::slotTimerEvent()
{
}
