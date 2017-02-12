#ifndef TVISUALWIDGET_H
#define TVISUALWIDGET_H

#include "../../share/skin.h"

#define BAND_COUNT 64

enum TVisualType
{
    VT_WAVE,
    VT_SPECTRUM_PILLAR,
    VT_SPECTRUM_BLOCK,
    VT_VIDEO,
    VisualTypeCount
};

class TVisualWidget : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TVisualWidget(QWidget *parent = 0);
    ~TVisualWidget();

    void setVisualType(TVisualType type);
    void setValue(float *data, int size=BAND_COUNT);
    void setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor=QColor());

    TVisualType visualType();

signals:

public slots:

private:
    float mSamleValues[BAND_COUNT];
    int mTopBlockValue[BAND_COUNT];
    int mTopBlockSpeed[BAND_COUNT];
    int mTopBlockBlank[BAND_COUNT];
    TVisualType mType;

    int mSpectrumWidth;
    int mSpectrumSpace;
    int mSpectrumTopBlockHeight;
    QColor mColorBlock;
    QColor mColorTop;
    QColor mColorMiddle;
    QColor mColorBottom;

    void caculateTiles();

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
