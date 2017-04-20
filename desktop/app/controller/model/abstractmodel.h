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
#ifndef TABSTRACTMODEL_H
#define TABSTRACTMODEL_H

#include "utils.h"

#include "../core/core.h"

class TAbstractModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TAbstractModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    int currentIndex();
    virtual void setCurrentIndex(int index);
    virtual void setPlayingIndex(int index);

    static void setFont(QFont font);
    static void setBackgroundColor(QColor color);
    static void setTextColor(QColor color);
    static void setCurrentRowTextColor(QColor color);
    static void setSelectedTextColor(QColor color);
    static void setColorParameters(QFont font,
                                   QColor textColor,
                                   QColor selectedTextColor,
                                   QColor numberColor,
                                   QColor durationColor,
                                   QColor currentRowTextColor,
                                   QColor backgroundColor,
                                   QColor backgroundColor2
                                   );

    virtual void moveItems(QList<int> indexes, int pos, QList<int> &indexesMoved) = 0;

    void update();
    void setPlayerCore(TCore *core);

signals:
    void requestAdjustColumnWidth();

private:
    static QVariant mFont;
    static QVariant mBackColor1;
    static QVariant mBackColor2;
    static QVariant mTextColor;
    static QVariant mCurrentRowTextColor;
    static QVariant mSelectedTextColor;
    static QVariant mNumberColor;
    static QVariant mDurationColor;

protected:
    int mCurrentIndex;
    int mPlayingIndex;
    TCore *mCore;

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const = 0;
    virtual int columnCount(const QModelIndex &parent) const = 0;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TABSTRACTMODEL_H
