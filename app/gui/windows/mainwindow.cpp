#include "mainwindow.h"

const int c_title_interval = 300;

TMainWindow::TMainWindow(QWidget *parent) : TAbstractWindow(parent)
    , mBtnPlay(new TImageButton(this))
    , mBtnPrev(new TImageButton(this))
    , mBtnNext(new TImageButton(this))
    , mBtnPause(new TImageButton(this))
    , mBtnStop(new TImageButton(this))
    , mBtnOpen(new TImageButton(this))
    , mBtnMute(new TImageButton(this))
    , mBtnMinimize(new TImageButton(this))
    , mBtnBrowser(new TImageButton(this))
    , mBtnEqualizer(new TImageButton(this))
    , mBtnPlaylist(new TImageButton(this))
    , mBtnLyrics(new TImageButton(this))
    , mBtnExit(new TImageButton(this))
    , mBtnMinimode(new TImageButton(this))
    , mProgressBar(new TSliderBar(Qt::Horizontal, this))
    , mVolumeBar(new TSliderBar(Qt::Horizontal, this))
    , mIcon(new TLabel(this))
    , mMusicTitle(new TScrollLabel(this))
    , mLedTime(new TLedWidget(this))
    , mStereo(new TLabel(this))
    , mStatus(new TLabel(this))
    , mVisualWidget(new TVisualWidget(this))
    , mCaptionIndex(0)
    , mTitleTimerId(-1)
    , mMinimode(false)
    , mWindowHided(false)
{
    setObjectName("MainWindow");

    setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint|Qt::WindowMinimizeButtonHint);

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

    connect(mBtnExit, SIGNAL(clicked()), this, SIGNAL(exitClicked()));
    connect(mBtnPlay, SIGNAL(clicked()), this, SLOT(on_btnPlay_clicked()));
    connect(mBtnPause, SIGNAL(clicked()), this, SLOT(on_btnPause_clicked()));
    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));
    connect(mBtnStop, SIGNAL(clicked()), this, SIGNAL(stopClicked()));
    connect(mBtnNext, SIGNAL(clicked()), this, SIGNAL(nextClicked()));
    connect(mBtnPrev, SIGNAL(clicked()), this, SIGNAL(prevClicked()));
    connect(mBtnMinimize, SIGNAL(clicked()), this, SIGNAL(requestShowMinimized()));
    connect(mBtnMinimode, SIGNAL(clicked()), this, SIGNAL(miniModeClicked()));
    connect(mBtnEqualizer, SIGNAL(clicked(bool)), this, SIGNAL(equalizerButtonToggle(bool)));
    connect(mBtnPlaylist, SIGNAL(clicked(bool)), this, SIGNAL(playlistButtonToggle(bool)));
    connect(mBtnLyrics, SIGNAL(clicked(bool)), this, SIGNAL(lyricButtonToggle(bool)));
    connect(mBtnBrowser, SIGNAL(clicked(bool)), this, SIGNAL(browserButtonToggle(bool)));
    connect(mBtnMute, SIGNAL(clicked(bool)), this, SLOT(on_btnMute_clicked()));
    connect(mVolumeBar, SIGNAL(valueChanged(int)), this, SLOT(on_volume_valueChanged(int)));
    connect(mProgressBar, SIGNAL(valueChanged(int)), this, SIGNAL(progressChanged(int)));

    mBtnBrowser->setVisible(false);
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

void TMainWindow::setProgress(int time, int total)
{
    if(mLedTime)
        mLedTime->setTime(time/1000, total/1000);

    if(mProgressBar)
    {
        mProgressBar->setMaximum(total);
        mProgressBar->setValue(time);
    }
}

void TMainWindow::setTitles(QStringList titles)
{
    if(mMusicTitle)
        mMusicTitle->setStrings(titles);
}

void TMainWindow::setCaption(QString title)
{
    mCaptionIndex = 0;
    mCaption = title;
    setWindowTitle(mCaption);
}

void TMainWindow::setPlayState(QString state)
{
    mPlayState = state;
    updatePlayStatus();
}

void TMainWindow::setEffect(QString effect)
{
    mPlayEffect = effect;
    updatePlayEffect();
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
    mContextMenu = menu;
}

void TMainWindow::setButtonPlayChecked(bool checked)
{
    if(mBtnPlay && mBtnPause)
    {
        mBtnPlay->setVisible(checked);
        mBtnPause->setVisible(!checked);
    }
}

void TMainWindow::retranslateUi()
{
    mBtnPause->setToolTip(tr("Pause(%1)").arg(mBtnPause->shortcut().toString()));
    mBtnPlay->setToolTip(tr("Play(%1)").arg(mBtnPlay->shortcut().toString()));
    mBtnBrowser->setToolTip(tr("Open browser(%1)").arg(mBtnBrowser->shortcut().toString()));
    mBtnMinimode->setToolTip(tr("Normal Mode(%1)").arg(mBtnMinimode->shortcut().toString()));
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

    updatePlayStatus();
    updatePlayEffect();
}

void TMainWindow::on_btnPlay_clicked()
{
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
    int minValue = mVolumeBar->minimum();
    int maxValue = mVolumeBar->maximum();
    float vf = (float)(value-minValue)/(maxValue-minValue);

    emit volumeValueChanged(vf);
    mVolumeBar->setToolTip(tr("Volume: %1%").arg(mVolumeBar->value()));
}

void TMainWindow::on_btnMute_clicked()
{
    emit volumeToggle(mBtnMute->isChecked());
    updatePlayEffect();
}

void TMainWindow::updatePlayStatus()
{
    mStatus->setText(mPlayState);
}

void TMainWindow::updatePlayEffect()
{
    if(mBtnMute->isChecked())
    {
        mStereo->setText(tr("Mute"));
    } else {
        if(mPlayEffect.isEmpty())
            mPlayEffect = tr("Stereo");
        mStereo->setText(mPlayEffect);
    }
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

void TMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(mContextMenu)
        mContextMenu->popup(event->globalPos());

    QMainWindow::contextMenuEvent(event);
}

void TMainWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnPlay->loadFromSkin(element.firstChildElement(TAG_PLAYER_PLAY), skin);
    mBtnPause->loadFromSkin(element.firstChildElement(TAG_PLAYER_PAUSE), skin);
    mBtnPrev->loadFromSkin(element.firstChildElement(TAG_PLAYER_PREV), skin);
    mBtnNext->loadFromSkin(element.firstChildElement(TAG_PLAYER_NEXT), skin);
    mBtnStop->loadFromSkin(element.firstChildElement(TAG_PLAYER_STOP), skin);
    mBtnOpen->loadFromSkin(element.firstChildElement(TAG_PLAYER_OPEN), skin);
    mBtnMute->loadFromSkin(element.firstChildElement(TAG_PLAYER_MUTE), skin);
    mBtnLyrics->loadFromSkin(element.firstChildElement(TAG_PLAYER_LYRIC), skin);
    mBtnEqualizer->loadFromSkin(element.firstChildElement(TAG_PLAYER_EQUALIZER), skin);
    mBtnPlaylist->loadFromSkin(element.firstChildElement(TAG_PLAYER_PLAYLIST), skin);
    mBtnBrowser->loadFromSkin(element.firstChildElement(TAG_PLAYER_BROWSER), skin);
    mBtnMinimize->loadFromSkin(element.firstChildElement(TAG_PLAYER_MINIMIZE), skin);
    mBtnMinimode->loadFromSkin(element.firstChildElement(TAG_PLAYER_MINIMODE), skin);
    mBtnExit->loadFromSkin(element.firstChildElement(TAG_PLAYER_EXIT), skin);
    mProgressBar->loadFromSkin(element.firstChildElement(TAG_PLAYER_PROGRESS), skin);
    mVolumeBar->loadFromSkin(element.firstChildElement(TAG_PLAYER_VOLUME), skin);
    mIcon->loadFromSkin(element.firstChildElement(TAG_PLAYER_ICON), skin);
    mMusicTitle->loadFromSkin(element.firstChildElement(TAG_PLAYER_INFO), skin);
    mLedTime->loadFromSkin(element.firstChildElement(TAG_PLAYER_LED), skin);
    mStereo->loadFromSkin(element.firstChildElement(TAG_PLAYER_STEREO), skin);
    mStatus->loadFromSkin(element.firstChildElement(TAG_PLAYER_STATUS), skin);
    mVisualWidget->loadFromSkin(element.firstChildElement(TAG_PLAYER_VISUAL), skin);

    QDomElement iconElement = element.firstChildElement(TAG_PLAYER_ICON);
    QIcon ico = skin->findIcon(iconElement.attribute(ATTR_ICON));
    setWindowIcon(ico);
}

void TMainWindow::changeEvent(QEvent *event)
{
    QEvent::Type et = event->type();
    if(et == QEvent::WindowStateChange)
    {
        Qt::WindowStates ws = windowState();
        if(ws==Qt::WindowMinimized)
            emit requestShowMinimized();
    } else if (et == QEvent::ActivationChange) {
        emit onActivationChange();
    }
}
