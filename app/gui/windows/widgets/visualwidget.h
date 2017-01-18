#ifndef TVISUALWIDGET_H
#define TVISUALWIDGET_H

#include "../../share/skin.h"

#define SAMPLE_SIZE 64

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
    void setValue(QList<int> data);
    void setValue(QVector<int> data);
    void setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor=QColor());
    int sampleSize()
    {
        return SAMPLE_SIZE;
    }
    int maxValue()
    {
        return height();
    }

signals:

public slots:

private:
    int mSamleValues[SAMPLE_SIZE];
    int mTopBlockValue[SAMPLE_SIZE];
    int mTopBlockSpeed[SAMPLE_SIZE];
    TVisualType mType;

    int mSpectrumWidth;
    int mSpectrumSpace;
    int mSpectrumTopBlockHeight;
    QColor mColorBlock;
    QColor mColorTop;
    QColor mColorMiddle;
    QColor mColorBottom;

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
