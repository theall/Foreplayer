#include "miniwindow.h"

TMiniWindow::TMiniWindow(QWidget *parent) : TAbstractWindow(parent)
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
{

}

TMiniWindow::~TMiniWindow()
{

}

void TMiniWindow::retranslateUi()
{
    mBtnMinimode->setToolTip(tr("Mini Mode(%1)").arg(mBtnMinimode->shortcut().toString()));
}

void TMiniWindow::loadFromSkin(QDomElement element, TSkin *skin)
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
}
