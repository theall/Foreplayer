#ifndef TVISUALWIDGET_H
#define TVISUALWIDGET_H

#include "../../share/skin.h"

#define BAND_COUNT      64
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
