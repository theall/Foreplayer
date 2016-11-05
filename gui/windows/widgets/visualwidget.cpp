#include "visualwidget.h"

TVisualWidget::TVisualWidget(QWidget *parent) :
    QWidget(parent)
{
    mType = VT_SPECTRUM_BLOCK;

    for(int i=0;i<SAMPLE_SIZE;i++)
        mSamleValues[i] = 0;

    for(int i=0;i<SAMPLE_SIZE;i++)
        mTopBlockValue[i] = 0;
}

TVisualWidget::~TVisualWidget()
{
}

void TVisualWidget::setVisualType(VisualType type)
{
    mType = type;
}

void TVisualWidget::setValue(QList<int> data)
{
    if(data.size() != SAMPLE_SIZE)
        return;

    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        mSamleValues[i] = data[i];
    }
}

void TVisualWidget::setValue(QVector<int> data)
{
    if(data.size() != SAMPLE_SIZE)
        return;

    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        mSamleValues[i] = data[i];
    }
    update();
}

void TVisualWidget::setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor)
{
    mColorBlock = blockColor;
    mColorTop = topColor;
    mColorMiddle = middleColor;
    mColorBottom = bottomColor;
}

void TVisualWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    int x = 0;
    QRect mainRect = rect();
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();
    int nWidth = SAMPLE_WIDTH + SAMPLE_SPACE;

    QBrush blockBrush(mColorBlock);

    QLinearGradient linear(0, SAMPLE_WIDTH/2, 0, height());
    linear.setColorAt(0, mColorTop);
    if(mColorMiddle.isValid())
        linear.setColorAt(0.5, mColorMiddle);
    linear.setColorAt(1, mColorBottom);

    QBrush pillarBrush(linear);

    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        if(x+SAMPLE_WIDTH>mainWidth)
            break;

        int specValue = mSamleValues[i];

        // draw spectrum pillar
        p.fillRect(x, mainHeight-specValue, SAMPLE_WIDTH, specValue, pillarBrush);

        int topBlockValue = mTopBlockValue[i];
        int topBlockSpeed = mTopBlockSpeed[i];

        // draw top block
        if(specValue >= topBlockValue)
        {
            topBlockSpeed = 0;
            topBlockValue = specValue;
        }
        else
        {
            topBlockSpeed++;
            topBlockValue -= topBlockSpeed;
        }

        int y = mainHeight-topBlockValue-TOP_BLOCK_HEIGHT;
        if(y>=0)
            // fill block
            p.fillRect(x, y, SAMPLE_WIDTH, TOP_BLOCK_HEIGHT, blockBrush);

        mTopBlockSpeed[i] = topBlockSpeed;
        mTopBlockValue[i] = topBlockValue;
        x += nWidth;
    }

    p.end();
}

void TVisualWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
}

void TVisualWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    if(event->button()==Qt::LeftButton)
    {
        mType = (VisualType)((int)mType+1);
        if(mType == VisualTypeCount)
            mType = VT_WAVE;
    }
}
