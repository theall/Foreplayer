#include "visualwidget.h"

#include <QtMath>
#define SAMPLE_WIDTH                3.0
#define SAMPLE_SPACE                1
#define SAMPLE_TOP_BLOCK_HEIGHT     1
#define SAMPLE_TOP_BLOCK_PAUSE_TIME 5

TVisualWidget::TVisualWidget(QWidget *parent) : QWidget(parent)
    , mType(VT_SPECTRUM_BLOCK)
    , mSpectrumWidth(SAMPLE_WIDTH)
    , mSpectrumSpace(SAMPLE_SPACE)
    , mSpectrumTopBlockHeight(SAMPLE_TOP_BLOCK_HEIGHT)
{
    for(int i=0;i<BAND_COUNT;i++)
        mSamleValues[i] = 0;

    for(int i=0;i<BAND_COUNT;i++)
    {
        mTopBlockValue[i] = 0;
        mTopBlockSpeed[i] = 0;
        mTopBlockBlank[i] = 0;
    }
}

TVisualWidget::~TVisualWidget()
{
}

void TVisualWidget::setVisualType(TVisualType type)
{
    mType = type;
}

void TVisualWidget::setValue(float *data, int size)
{
    if(size != BAND_COUNT)
        return;

    memcpy(mSamleValues, data, BAND_COUNT*sizeof(float));
    caculateTiles();
}

void TVisualWidget::setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor)
{
    mColorBlock = blockColor;
    mColorTop = topColor;
    mColorMiddle = middleColor;
    mColorBottom = bottomColor;
}

TVisualType TVisualWidget::visualType()
{
    return mType;
}

void TVisualWidget::caculateTiles()
{
    int mainHeight = rect().height();
    for(int i=0;i<BAND_COUNT;i++)
    {
        float realHeight = mSamleValues[i]*mainHeight;
        if(realHeight < 0)
            realHeight = 0;
        else if (realHeight > mainHeight)
            realHeight = mainHeight;
        mSamleValues[i] = realHeight;
    }
    update();
}

void TVisualWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), QBrush());

    QRect mainRect = rect();
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();
    int nWidth = mSpectrumWidth + mSpectrumSpace;

    QBrush blockBrush(mColorBlock);

    QLinearGradient linear(0, mSpectrumWidth/2, 0, height());
    linear.setColorAt(0, mColorTop);
    if(mColorMiddle.isValid())
        linear.setColorAt(0.5, mColorMiddle);
    linear.setColorAt(1, mColorBottom);

    QBrush pillarBrush(linear);
    int offsetX = 0;
    for(int i=0;i<BAND_COUNT;i++)
    {
        if(offsetX+mSpectrumWidth>mainWidth)
            break;

        int realSpecValue = mSamleValues[i];

        // draw spectrum pillar
        painter.fillRect(offsetX, mainHeight-realSpecValue, mSpectrumWidth, realSpecValue, pillarBrush);

        int topBlockValue = mTopBlockValue[i];
        int topBlockSpeed = mTopBlockSpeed[i];

        // draw top block
        if(realSpecValue >= topBlockValue)
        {
            topBlockSpeed = 0;
            mTopBlockBlank[i] = SAMPLE_TOP_BLOCK_PAUSE_TIME;
            topBlockValue = realSpecValue;
        }
        else
        {
            if(mTopBlockBlank[i] > 0)
                mTopBlockBlank[i]--;
            else
            {
                topBlockSpeed++;
                topBlockValue -= topBlockSpeed;
            }
        }

        if(topBlockValue < 0)
            topBlockValue = 0;
        if(topBlockValue > mainHeight+mSpectrumTopBlockHeight)
            topBlockValue = mainHeight;
        // fill block
        painter.fillRect(offsetX, mainHeight-topBlockValue-mSpectrumTopBlockHeight, mSpectrumWidth, mSpectrumTopBlockHeight, blockBrush);

        mTopBlockSpeed[i] = topBlockSpeed;
        mTopBlockValue[i] = topBlockValue;
        offsetX += nWidth;
    }

    painter.end();
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
        mType = (TVisualType)((int)mType+1);
        if(mType == VisualTypeCount)
            mType = VT_WAVE;
    }
}

void TVisualWidget::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    Q_UNUSED(skin)

    setGeometry(SkinUtils::extractGeometry(element));
    mColorBlock = QColor(element.attribute(ATTR_BLOCK_COLOR));
    mColorTop = QColor(element.attribute(ATTR_TOP_COLOR));
    mColorMiddle = QColor(element.attribute(ATTR_MIDDLE_COLOR));
    mColorBottom = QColor(element.attribute(ATTR_BOTTOM_COLOR));
}
