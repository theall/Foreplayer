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
#ifndef TVISUALWIDGET_H
#define TVISUALWIDGET_H

#include "../../share/skin.h"

#define LEVEL_COUNT     4096

enum TVisualType
{
    VT_SPECTRUM_BLOCK,
    VT_SPECTRUM_LINE,
    VT_WAVE_IMPULSE,
    VT_WAVE_OSCILLOGRAM,
    VT_VIDEO,
    VT_HIDDEN,
    VisualTypeCount
};

class TVisualWidget : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TVisualWidget(QWidget *parent = 0);
    ~TVisualWidget();

    void setVisualType(TVisualType type);
    void setValue(float *data, int size);
    void setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor=QColor());
    void reset();

    bool spectrumMode();
    void setSwitchOnClick(bool enable);

    TVisualType visualType();

signals:

public slots:

private:
    int mSampleCount;
    bool mSwitchOnClick;
    float mSamleValues[LEVEL_COUNT];
    int mTopBlockValue[LEVEL_COUNT];
    int mTopBlockSpeed[LEVEL_COUNT];
    int mTopBlockBlank[LEVEL_COUNT];
    TVisualType mVisualType;

    int mSpectrumWidth;
    int mSpectrumSpace;
    int mSpectrumTopBlockHeight;
    QColor mColorBlock;
    QColor mColorTop;
    QColor mColorMiddle;
    QColor mColorBottom;

    int mBandCount;

    void caculateTiles();
    void drawSpectrumBlock(QPainter *painter, QRect mainRect, QBrush pillarBrush);
    void drawSpectrumLine(QPainter *painter, QRect mainRect, QBrush pillarBrush);
    void drawWaveImpulse(QPainter *painter, QRect mainRect, QBrush pillarBrush);
    void drawWaveOsci(QPainter *painter, QRect mainRect, QBrush pillarBrush);
    void drawVideo(QPainter *painter, QRect mainRect, QBrush pillarBrush);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TVISUALWIDGET_H
