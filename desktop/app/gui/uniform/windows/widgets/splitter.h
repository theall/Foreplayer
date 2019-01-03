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
#ifndef TSPLITTER_H
#define TSPLITTER_H

#include "imagebutton.h"

class TSplitterHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    TSplitterHandle(int index, QSplitter *parent = 0);

    /*
     * When splitter move from collapsed, set the handle to expand.
     */
    void setHandleStatus(bool collapsed);

signals:
    void requestCollapse(int index, bool collapse);

private:
    int mIndex;
    bool mCollapsed;
    bool mouseInButton(int pos);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void hideEvent(QHideEvent *) Q_DECL_OVERRIDE;
};

class TSplitter : public QSplitter
{
    Q_OBJECT

public:
    TSplitter(QWidget *parent = 0);

private slots:
    void slotCollapseWidget(int mIndex, bool collapse);
    void slotSplitterMoved(int pos, int index);

private:
    int mIndex;
    QMap<QWidget*, int> mWidgetWidth;

protected:
    QSplitterHandle *createHandle() Q_DECL_OVERRIDE;
};

#endif // TSPLITTER_H
