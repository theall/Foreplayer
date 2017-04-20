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
#include "abstractmodel.h"

#define BACKGROUND2_ALPHA 128

QVariant TAbstractModel::mFont;
QVariant TAbstractModel::mBackColor1;
QVariant TAbstractModel::mBackColor2;
QVariant TAbstractModel::mTextColor;
QVariant TAbstractModel::mCurrentRowTextColor;
QVariant TAbstractModel::mSelectedTextColor;
QVariant TAbstractModel::mNumberColor;
QVariant TAbstractModel::mDurationColor;

TAbstractModel::TAbstractModel(QObject *parent) :
    QAbstractTableModel(parent),
    mCurrentIndex(-1),
    mPlayingIndex(-1),
    mCore(NULL)
{
}

QVariant TAbstractModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::BackgroundRole)
    {
        if(index.row()%2 == 1)
            return TAbstractModel::mBackColor2;
        return TAbstractModel::mBackColor1;
    } else if (role==Qt::FontRole) {
        return TAbstractModel::mFont;
    } else if (role==Qt::TextColorRole) {
        return TAbstractModel::mTextColor;
    } else if (role==Utils::TextHighlight) {
        return TAbstractModel::mSelectedTextColor;
    } else if (role==Utils::IsPlayingItem) {
        return index.row()==mPlayingIndex;
    } else if (role==Utils::IsCurrentRow) {
        return index.row()==mCurrentIndex;
    } else {
        return QVariant();
    }

    return QVariant();
}

int TAbstractModel::currentIndex()
{
    return mCurrentIndex;
}

void TAbstractModel::setCurrentIndex(int index)
{
    if(mCurrentIndex == index)
        return;

    mCurrentIndex = index;

    QVector<int> roles;
    roles.append(Qt::TextColorRole);
    roles.append(Utils::IsCurrentRow);
    emit dataChanged(QModelIndex(), QModelIndex(), roles);
}

void TAbstractModel::setPlayingIndex(int index)
{
    mPlayingIndex = index;

    emit dataChanged(QModelIndex(), QModelIndex());
}

void TAbstractModel::setFont(QFont font)
{
    mFont = font;
}

void TAbstractModel::setBackgroundColor(QColor color)
{
    color.setAlpha(0);
    mBackColor1 = color;
    color.setAlpha(BACKGROUND2_ALPHA);
    mBackColor2 = color;
}

void TAbstractModel::setTextColor(QColor color)
{
    mTextColor = color;
}

void TAbstractModel::setCurrentRowTextColor(QColor color)
{
    mCurrentRowTextColor = color;
}

void TAbstractModel::setSelectedTextColor(QColor color)
{
    mSelectedTextColor = color;
}

void TAbstractModel::setColorParameters(QFont font,
                                        QColor textColor,
                                        QColor selectedTextColor,
                                        QColor numberColor,
                                        QColor durationColor,
                                        QColor currentRowTextColor,
                                        QColor backgroundColor,
                                        QColor backgroundColor2)
{
    mFont = font;
    backgroundColor.setAlpha(0);
    mBackColor1 = backgroundColor;
    backgroundColor2.setAlpha(BACKGROUND2_ALPHA);
    mBackColor2 = backgroundColor2;
    mTextColor = textColor;
    mCurrentRowTextColor = currentRowTextColor;
    mSelectedTextColor = selectedTextColor;
    mNumberColor = numberColor;
    mDurationColor = durationColor;
}

void TAbstractModel::update()
{
    emit layoutChanged();
}

void TAbstractModel::setPlayerCore(TCore *core)
{
    mCore = core;
}

Qt::ItemFlags TAbstractModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}
