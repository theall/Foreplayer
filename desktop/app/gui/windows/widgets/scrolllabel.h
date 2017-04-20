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
#ifndef TSCROLLLABEL_H
#define TSCROLLLABEL_H

#include "../../share/skin.h"

#include <QMutex>

class TScrollLabel : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TScrollLabel(QWidget *parent = 0);

    void setStrings(QStringList strlist);
    void setFontColor(QFont font, QColor color);
    void setSwitchOnClick(bool enabled);
    void setSpeed(int factor);

signals:

public slots:

// QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *event);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    enum State {
        NONE,
        LINE_DISPLAYING,
        LINE_SWITCHING,
        SHUTTLE_WAIT_START,
        SHUTTLING_LEFT,
        SHUTTLE_PAUSED_IDLE,
        SHUTTLING_RIGHT,
        SHUTTLE_END_IDLE
    };
    int mUpdateInterval;
    int mSwitchInterval;
    int mShuttleInterval;
    QMutex mMutex;
    bool mSwitchOnClick;
    bool mSwitchEnd;
    State mState;
    int mTimerID;
    int mIdleTime;
    int mOffsetX, mOffsetY;
    QStringList mStringList;
    QList<int> mLineWidths;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TSCROLLLABEL_H
