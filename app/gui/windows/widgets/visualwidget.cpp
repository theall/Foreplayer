#include "visualwidget.h"

#include <QtMath>
#define SAMPLE_WIDTH 3.0
#define SAMPLE_SPACE 1
#define SAMPLE_TOP_BLOCK_HEIGHT 1

TVisualWidget::TVisualWidget(QWidget *parent) : QWidget(parent)
    , mType(VT_SPECTRUM_BLOCK)
    , mSpectrumWidth(SAMPLE_WIDTH)
    , mSpectrumSpace(SAMPLE_SPACE)
    , mSpectrumTopBlockHeight(SAMPLE_TOP_BLOCK_HEIGHT)
{
    for(int i=0;i<SAMPLE_SIZE;i++)
        mSamleValues[i] = 0;

    for(int i=0;i<SAMPLE_SIZE;i++)
        mTopBlockValue[i] = 0;
}

TVisualWidget::~TVisualWidget()
{
}

void TVisualWidget::setVisualType(TVisualType type)
{
    mType = type;
}

void TVisualWidget::setValue(QList<byte> data)
{
    if(data.size() != SAMPLE_SIZE)
        return;

    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        mSamleValues[i] = data[i];
    }
    update();
}

void TVisualWidget::setValue(QByteArray data)
{
    int dataSize = data.size();
    int blockSize = qCeil((double)dataSize/SAMPLE_SIZE);
    int currentIndex = 0;
    int forSpectrum = 0;
    int index = 0;
    for(int i=0;i<data.size();i++)
    {
        forSpectrum += data[i];
        currentIndex++;
        if((currentIndex+1) >= blockSize)
        {
            mSamleValues[index++] = forSpectrum / blockSize;
            currentIndex = 0;
        }
    }
    update();
}

void TVisualWidget::setValue(QVector<byte> data)
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
    int nWidth = mSpectrumWidth + mSpectrumSpace;

    QBrush blockBrush(mColorBlock);

    QLinearGradient linear(0, mSpectrumWidth/2, 0, height());
    linear.setColorAt(0, mColorTop);
    if(mColorMiddle.isValid())
        linear.setColorAt(0.5, mColorMiddle);
    linear.setColorAt(1, mColorBottom);

    QBrush pillarBrush(linear);

    for(int i=0;i<SAMPLE_SIZE;i++)
    {
        if(x+mSpectrumWidth>mainWidth)
            break;

        byte specValue = mSamleValues[i];
        int realSpecValue = ((float)specValue/0xff)*mainHeight;

        // draw spectrum pillar
        p.fillRect(x, mainHeight-realSpecValue, mSpectrumWidth, realSpecValue, pillarBrush);

        int topBlockValue = mTopBlockValue[i];
        int topBlockSpeed = mTopBlockSpeed[i];

        // draw top block
        if(realSpecValue >= topBlockValue)
        {
            topBlockSpeed = 0;
            topBlockValue = realSpecValue;
        }
        else
        {
            topBlockSpeed++;
            topBlockValue -= topBlockSpeed;
        }

        int y = mainHeight-topBlockValue-mSpectrumTopBlockHeight;
        if(y>=0)
            // fill block
            p.fillRect(x, y, mSpectrumWidth, mSpectrumTopBlockHeight, blockBrush);

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
