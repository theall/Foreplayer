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
#include "playlistwidget.h"

#include <QVBoxLayout>

TPlaylistWidget::TPlaylistWidget(QWidget *parent) :
    QWidget(parent),
    mMinWidth(100),
    mMinHeight(60),
    mMarginLeft(0),
    mMarginTop(0),
    mMarginRight(0),
    mMarginBottom(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    mPlaylistView = new TPlaylistView(this);
    mMusiclistView = new TMusiclistView(this);
    mTracklistView = new TTracklistView(this);
    mSplitter = new TSplitter(this);
    mSplitter->addWidget(mPlaylistView);
    mSplitter->addWidget(mMusiclistView);
    mSplitter->addWidget(mTracklistView);

    mSplitter->setStretchFactor(0, 2);
    mSplitter->setStretchFactor(1, 4);
    mSplitter->setStretchFactor(2, 4);
    layout->addWidget(mSplitter);

    setLayout(layout);
}

TPlaylistWidget::~TPlaylistWidget()
{

}

void TPlaylistWidget::setAlignment(QPixmap background, QRect rect)
{
    mMarginLeft = rect.left();
    mMarginTop = rect.top();
    mMarginRight = background.width() - rect.right() + 1;
    mMarginBottom = background.height() - rect.bottom() + 1;
    mMinWidth = rect.width();
    mMinHeight = rect.height();
}

bool TPlaylistWidget::updatePosition(QSize size)
{
    int newRight = size.width() - mMarginRight;
    int newBottom = size.height() - mMarginBottom;
    int newWidth = newRight - mMarginLeft;
    int newHeight = newBottom - mMarginTop;
    QRect newGeometry(mMarginLeft, mMarginTop, newWidth, newHeight);
    bool canResize = false;
    if(newWidth >= mMinWidth)
    {
        newGeometry.setRight(newRight);
        canResize = true;
    }
    if(newHeight >= mMinHeight)
    {
        newGeometry.setBottom(newBottom);
        canResize = true;
    }

    setGeometry(newGeometry);

    return canResize;
}

void TPlaylistWidget::setFontColors(QColor background2)
{
    TAbstractTableView::setBackgroundColor(background2);
}

void TPlaylistWidget::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    QDomElement parentElement = element.parentNode().toElement();
    setAlignment(skin->findPixmap(parentElement.attribute(ATTR_IMAGE)), SkinUtils::extractGeometry(element));

    mTextFont.fromString(element.attribute(ATTR_FONT));
    mColorText.setNamedColor(element.attribute(ATTR_COLOR_TEXT));
    mColorHilight.setNamedColor(element.attribute(ATTR_COLOR_HILIGHT));
    mColorBkgnd.setNamedColor(element.attribute(ATTR_COLOR_BKGND));
    mColorNumber.setNamedColor(element.attribute(ATTR_COLOR_NUMBER));
    mColorDuration.setNamedColor(element.attribute(ATTR_COLOR_DURATION));
    mColorSelect.setNamedColor(element.attribute(ATTR_COLOR_SELECT));
    mColorBkgnd2.setNamedColor(element.attribute(ATTR_COLOR_BKGND2));
    TAbstractTableView::setBackgroundColor(mColorBkgnd2);
    mSelectedPixmap = skin->findPixmap(element.attribute(ATTR_SELECTED_IMAGE));
    TTableViewDelegate::setSelectedPixmap(&mSelectedPixmap);
}
