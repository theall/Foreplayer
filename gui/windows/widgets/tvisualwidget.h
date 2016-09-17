#ifndef TVISUALWIDGET_H
#define TVISUALWIDGET_H

#include "pch.h"

#define SAMPLE_WIDTH 3.0
#define SAMPLE_SPACE 1
#define SAMPLE_SIZE 64
#define TOP_BLOCK_HEIGHT 1

enum VisualType
{
    VT_WAVE,
    VT_SPECTRUM_PILLAR,
    VT_SPECTRUM_BLOCK,
    VT_VIDEO,
    VisualTypeCount
};

class TVisualWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TVisualWidget(QWidget *parent = 0);
    ~TVisualWidget();

    void setVisualType(VisualType type);
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
    VisualType mType;

    QColor mColorBlock;
    QColor mColorTop;
    QColor mColorMiddle;
    QColor mColorBottom;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // TVISUALWIDGET_H
