/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "browserwindow.h"

TBrowserWindow::TBrowserWindow(QWidget *parent) : TAbstractWindow(parent)
{
    setObjectName("BrowserWindow");

    mBtnClose = new TImageButton(this);
    mBtnBackward = new TImageButton(this);
    mBtnForward = new TImageButton(this);
    mBtnRefresh = new TImageButton(this);
    mCkbStartup = new TCheckBox(this);
    mLabelLink = new TLinkLabel(this);
    mBtnBrowser = new TImageButton(this);
    mBtnMini = new TImageButton(this);
    mBtnDownload = new TImageButton(this);

    mCkbStartup->setCheckable(true);

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));

    retranslateUi();
}

TBrowserWindow::~TBrowserWindow()
{

}

void TBrowserWindow::on_btnClose_clicked()
{
    hide();

    emit browserWindowToggled(false);
}

void TBrowserWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnClose->loadFromSkin(element.firstChildElement(TAG_BROWSER_CLOSE), skin);
    mBtnBackward->loadFromSkin(element.firstChildElement(TAG_BROWSER_BACKWARD), skin);
    mBtnForward->loadFromSkin(element.firstChildElement(TAG_BROWSER_FORWARD), skin);
    mBtnRefresh->loadFromSkin(element.firstChildElement(TAG_BROWSER_REFRESH), skin);
    mCkbStartup->loadFromSkin(element.firstChildElement(TAG_BROWSER_STARTUP), skin);
    mLabelLink->loadFromSkin(element.firstChildElement(TAG_BROWSER_LINKTXT), skin);
    mBtnBrowser->loadFromSkin(element.firstChildElement(TAG_BROWSER_BROWSER), skin);
    mBtnMini->loadFromSkin(element.firstChildElement(TAG_BROWSER_MINIBTN), skin);
    mBtnDownload->loadFromSkin(element.firstChildElement(TAG_BROWSER_DOWNLOAD_BUTTON), skin);

    update();
}

void TBrowserWindow::retranslateUi()
{
    mBtnClose->setToolTip(tr("Close"));
    mBtnBackward->setToolTip(tr("Backward"));
    mBtnForward->setToolTip(tr("Forward"));
    mBtnRefresh->setToolTip(tr("Refresh"));
    mCkbStartup->setToolTip(tr("Diasplay on startup"));
    mLabelLink->setText(tr("Use instructions"));
    mBtnBrowser->setToolTip(tr("Browser"));
    mBtnMini->setToolTip(tr("Mini"));
    mBtnDownload->setToolTip(tr("Download"));
}
