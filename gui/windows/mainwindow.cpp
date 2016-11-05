#include "mainwindow.h"

const int c_title_interval = 300;

TMainWindow::TMainWindow(QWidget *parent)
    : TAbstractWindow(parent),
      mCaptionIndex(0),
      mTitleTimerId(-1),
      mMinimode(false),
      mWindowHided(false)
{
    setObjectName("MainWindow");

    mBtnPlay = new TImageButton(this);
    mBtnPrev = new TImageButton(this);
    mBtnNext = new TImageButton(this);
    mBtnPause = new TImageButton(this);
    mBtnStop = new TImageButton(this);
    mBtnOpen = new TImageButton(this);
    mBtnMute = new TImageButton(this);
    mBtnMinimize = new TImageButton(this);
    mBtnBrowser = new TImageButton(this);
    mBtnEqualizer = new TImageButton(this);
    mBtnPlaylist = new TImageButton(this);
    mBtnLyrics = new TImageButton(this);
    mBtnExit = new TImageButton(this);
    mBtnMinimode = new TImageButton(this);
    mProgressBar = new TSliderBar(this);
    mVolumeBar = new TSliderBar(this);
    mIcon = new TLabel(this);
    mMusicTitle = new TScrollLabel(this);
    mLedTime = new TLedWidget(this);
    mStereo = new TLabel(this);
    mStatus = new TLabel(this);
    mVisualWidget = new TVisualWidget(this);
    mTrayIcon = new QSystemTrayIcon(this);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mBtnPlay->setShortcut(QKeySequence(Qt::Key_F5));

    mBtnPause->setShortcut(QKeySequence(Qt::Key_F5));
    mBtnPause->setVisible(false);

    mBtnPrev->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Left));
    mBtnNext->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Right));
    mBtnStop->setShortcut(QKeySequence(Qt::Key_F6));
    mBtnOpen->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_O));

    mBtnMute->setCheckable(true);
    mBtnMute->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_S));

    mBtnBrowser->setCheckable(true);
    mBtnBrowser->setShortcut(QKeySequence(Qt::Key_F11));

    mBtnPlaylist->setCheckable(true);
    mBtnPlaylist->setShortcut(QKeySequence(Qt::Key_F4));

    mBtnLyrics->setCheckable(true);
    mBtnLyrics->setShortcut(QKeySequence(Qt::Key_F2));

    mBtnEqualizer->setCheckable(true);
    mBtnEqualizer->setShortcut(QKeySequence(Qt::Key_F3));

    mBtnMinimode->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_M));

    mIcon->setScaledContents(true);
    mIcon->setCursor(Qt::PointingHandCursor);

    connect(mBtnExit, SIGNAL(clicked()), this, SLOT(close()));
    connect(mBtnPlay, SIGNAL(clicked()), this, SLOT(on_btnPlay_clicked()));
    connect(mBtnPause, SIGNAL(clicked()), this, SLOT(on_btnPause_clicked()));
    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));
    connect(mBtnStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(mBtnNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(mBtnPrev, SIGNAL(clicked()), this, SIGNAL(prevClicked()));
    connect(mBtnMinimize, SIGNAL(clicked()), this, SIGNAL(requestShowMinimized()));
    connect(mBtnMinimode, SIGNAL(clicked()), this, SLOT(on_minimode_Clicked()));
    connect(mBtnEqualizer, SIGNAL(clicked(bool)), this, SIGNAL(equalizerButtonToggle(bool)));
    connect(mBtnPlaylist, SIGNAL(clicked(bool)), this, SIGNAL(playlistButtonToggle(bool)));
    connect(mBtnLyrics, SIGNAL(clicked(bool)), this, SIGNAL(lyricButtonToggle(bool)));
    connect(mBtnBrowser, SIGNAL(clicked(bool)), this, SIGNAL(browserButtonToggle(bool)));
    connect(mBtnMute, SIGNAL(clicked(bool)), this, SIGNAL(volumeToggle(bool)));
    connect(mVolumeBar, SIGNAL(valueChanged(int)), this, SLOT(on_volume_valueChanged(int)));
    connect(mProgressBar, SIGNAL(valueChanged(int)), this, SIGNAL(progressChanged(int)));

    // Tray icon
    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotTrayIconActivated(QSystemTrayIcon::ActivationReason)));

    retranslateUi();
}

TMainWindow::~TMainWindow()
{
    if(mTitleTimerId != -1)
    {
        killTimer(mTitleTimerId);
        mTitleTimerId = -1;
    }
}

void TMainWindow::setGuiParameter(PlayerWindowParam *mainParam, PlayerWindowParam *miniParam)
{
    mMainParam = *mainParam;
    mMiniParam = *miniParam;

    updateGuiParameter();
    mTrayIcon->show();
}

void TMainWindow::setTime(int time, int total)
{
    mLedTime->setTime(time, total);
}

void TMainWindow::setTitles(QStringList titles)
{
    mMusicTitle->setStrings(titles);
}

void TMainWindow::setCaption(QString title)
{
    mCaption = tr("Foreplayer %1 - ").arg(title);
    mCaptionIndex = 0;
}

void TMainWindow::checkLyricButton(bool checked)
{
    mBtnLyrics->setChecked(checked);
}

void TMainWindow::checkEqualizerButton(bool checked)
{
    mBtnEqualizer->setChecked(checked);
}

void TMainWindow::checkPlaylistButton(bool checked)
{
    mBtnPlaylist->setChecked(checked);
}

void TMainWindow::checkBrowserButton(bool checked)
{
    mBtnBrowser->setChecked(checked);
}

void TMainWindow::setContextMenu(QMenu *menu)
{
    mTrayIcon->setContextMenu(menu);
    mContextMenu = menu;
}

void TMainWindow::retranslateUi()
{
    mBtnPause->setToolTip(tr("Pause(%1)").arg(mBtnPause->shortcut().toString()));
    mBtnPlay->setToolTip(tr("Play(%1)").arg(mBtnPlay->shortcut().toString()));
    mBtnBrowser->setToolTip(tr("Open browser(%1)").arg(mBtnBrowser->shortcut().toString()));

    updateMinimodeTooltip();

    mBtnOpen->setToolTip(tr("Open music(%1)").arg(mBtnOpen->shortcut().toString()));
    mBtnMute->setToolTip(tr("Volume switch(%1)").arg(mBtnMute->shortcut().toString()));
    mBtnPrev->setToolTip(tr("Previous(%1)").arg(mBtnPrev->shortcut().toString()));
    mBtnEqualizer->setToolTip(tr("Equalizer(%1)").arg(mBtnEqualizer->shortcut().toString()));
    mBtnStop->setToolTip(tr("Stop(%1)").arg(mBtnStop->shortcut().toString()));
    mBtnExit->setToolTip(tr("Exit"));
    mBtnNext->setToolTip(tr("Next(%1)").arg(mBtnNext->shortcut().toString()));
    mBtnMinimize->setToolTip(tr("Minimize"));
    mBtnPlaylist->setToolTip(tr("Playlist(%1)").arg(mBtnPlaylist->shortcut().toString()));
    mBtnLyrics->setToolTip(tr("Lyric(%1)").arg(mBtnLyrics->shortcut().toString()));
    mIcon->setToolTip(tr("Main menu"));
    mProgressBar->setToolTip(tr("Progress"));
    mVolumeBar->setToolTip(tr("Volume: %1%").arg(mVolumeBar->value()));

    mStatus->setText(tr("Status: Play"));
    mStereo->setText(tr("Stereo"));
    mTrayIcon->setToolTip("- Foreplayer");
}

void TMainWindow::on_btnPlay_clicked()
{
    mBtnPlay->setVisible(false);
    mBtnPause->setVisible(true);
    if(mTitleTimerId==-1)
        mTitleTimerId = startTimer(c_title_interval);

    emit playClicked();
}

void TMainWindow::on_btnPause_clicked()
{
    mBtnPlay->setVisible(true);
    mBtnPause->setVisible(false);
    if(mTitleTimerId != -1)
    {
        killTimer(mTitleTimerId);
        mTitleTimerId = -1;
    }

    emit pauseClicked();
}

void TMainWindow::on_volume_valueChanged(int value)
{
    mVolumeBar->setToolTip(tr("Volume: %1%").arg(mVolumeBar->value()));
    emit volumeValueChanged(value);
}

void TMainWindow::on_minimode_Clicked()
{
    mMinimode = !mMinimode;

    updateGuiParameter();

    emit showModeSwitch(mMinimode);
}

void TMainWindow::updateGuiParameter()
{
    PlayerWindowParam *param = mMinimode ? &mMiniParam : &mMainParam;

    setWindowParam(&param->window);

    setWindowIcon(param->icon.icon);
    mTrayIcon->setIcon(param->icon.icon);

    mBtnPlay->setPixmapRect(param->play.image, param->play.position.geometry());
    mBtnPrev->setPixmapRect(param->prev.image, param->prev.position.geometry());
    mBtnNext->setPixmapRect(param->next.image, param->next.position.geometry());
    mBtnPause->setPixmapRect(param->pause.image, param->pause.position.geometry());
    mBtnStop->setPixmapRect(param->stop.image, param->stop.position.geometry());
    mBtnOpen->setPixmapRect(param->open.image, param->open.position.geometry());
    mBtnMute->setPixmapRect(param->mute.image, param->mute.position.geometry());
    mBtnMinimize->setPixmapRect(param->minimize.image, param->minimize.position.geometry());
    mBtnBrowser->setPixmapRect(param->browser.image, param->browser.position.geometry());
    mBtnEqualizer->setPixmapRect(param->equalizer.image, param->equalizer.position.geometry());
    mBtnPlaylist->setPixmapRect(param->playlist.image, param->playlist.position.geometry());
    mBtnLyrics->setPixmapRect(param->lyric.image, param->lyric.position.geometry());
    mBtnExit->setPixmapRect(param->exit.image, param->exit.position.geometry());
    mBtnMinimode->setPixmapRect(param->minimode.image, param->minimode.position.geometry());

    mProgressBar->setGeometry(param->progress.position.geometry());
    mProgressBar->setPixmaps(param->progress.image, param->progress.image1, param->progress.image2);

    mVolumeBar->setGeometry(param->volume.position.geometry());
    mVolumeBar->setPixmaps(param->volume.image, param->volume.image1, param->volume.image2);
    mVolumeBar->setVertical(param->volume.vertical);

    mIcon->setGeometry(param->icon.position.geometry());
    mIcon->setPixmap(param->icon.image);

    mMusicTitle->setGeometry(param->info.position.geometry());
    mMusicTitle->setFontColor(param->info.font, param->info.color);

    mLedTime->setGeometry(param->led.position.geometry());
    mLedTime->setPixmap(param->led.image);
    mLedTime->setAlignment(param->led.alignment);

    mStereo->setGeometry(param->stereo.position.geometry());
    mStereo->setFontColor(param->stereo.font, param->stereo.color);
    mStereo->setAlignment(param->stereo.alignment);

    mStatus->setGeometry(param->status.position.geometry());
    mStatus->setFontColor(param->status.font, param->status.color);
    mStatus->setAlignment(param->status.alignment);

    mVisualWidget->setGeometry(param->visual.position.geometry());
    mVisualWidget->setColor(QColor(192,192,192), Qt::white, QColor(25,77,92), QColor(25,77,92));

    updateMinimodeTooltip();
    update();
}

void TMainWindow::updateMinimodeTooltip()
{
    if(mMinimode)
        mBtnMinimode->setToolTip(tr("Normal Mode(%1)").arg(mBtnMinimode->shortcut().toString()));
    else
        mBtnMinimode->setToolTip(tr("Mini Mode(%1)").arg(mBtnMinimode->shortcut().toString()));
}

void TMainWindow::on_btnOpen_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            tr("Select one or more files to open"),
                            "/",
                            tr("Musics (*.mp3 *.wma *.zip)"));
    if(files.isEmpty())
        return;
    emit requestOpenFiles(files);
}

void TMainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    QString s = mCaption.mid(mCaptionIndex) + mCaption.left(mCaptionIndex);
    setWindowTitle(s);

    mCaptionIndex++;
    if(mCaptionIndex >= mCaption.size())
        mCaptionIndex = 0;
}

void TMainWindow::slotTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason==QSystemTrayIcon::Trigger)
    {
        emit requestToggleWindow();
    } else if(reason==QSystemTrayIcon::DoubleClick) {
        emit requestRestoreWindow();
    }
}

void TMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(mContextMenu)
        mContextMenu->popup(event->globalPos());

    QMainWindow::contextMenuEvent(event);
}
