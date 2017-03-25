#include "visualwidget.h"

#include <QtMath>
#define SAMPLE_WIDTH                3.0
#define SAMPLE_SPACE                1
#define SAMPLE_TOP_BLOCK_HEIGHT     1
#define SAMPLE_TOP_BLOCK_PAUSE_TIME 15
#define DEFAULT_COLOR               "#FFFFFF"
#define BAND_COUNT_DEFAULT          64

TVisualWidget::TVisualWidget(QWidget *parent) : QWidget(parent)
  , mSampleCount(0)
  , mSwitchOnClick(true)
  , mVisualType(VT_SPECTRUM_BLOCK)
  , mSpectrumWidth(SAMPLE_WIDTH)
  , mSpectrumSpace(SAMPLE_SPACE)
  , mSpectrumTopBlockHeight(SAMPLE_TOP_BLOCK_HEIGHT)
  , mBandCount(BAND_COUNT_DEFAULT)
{
    for(int i=0;i<LEVEL_COUNT;i++)
        mSamleValues[i] = 0;

    for(int i=0;i<LEVEL_COUNT;i++)
    {
        mTopBlockValue[i] = 0;
        mTopBlockSpeed[i] = 0;
        mTopBlockBlank[i] = 0;
    }

    // Default color
    mColorBlock.setNamedColor(DEFAULT_COLOR);
    mColorTop.setNamedColor(DEFAULT_COLOR);
    mColorMiddle.setNamedColor(DEFAULT_COLOR);
    mColorBottom.setNamedColor(DEFAULT_COLOR);
}

TVisualWidget::~TVisualWidget()
{
}

void TVisualWidget::setVisualType(TVisualType type)
{
    mVisualType = type;
}

void TVisualWidget::setValue(float *data, int size)
{
    if(!data || size<1)
        return;

    mSampleCount = qMin(size, LEVEL_COUNT);
    memcpy(mSamleValues, data, mSampleCount*sizeof(float));

    caculateTiles();
}

void TVisualWidget::setColor(QColor blockColor, QColor topColor, QColor bottomColor, QColor middleColor)
{
    mColorBlock = blockColor;
    mColorTop = topColor;
    mColorMiddle = middleColor;
    mColorBottom = bottomColor;
}

void TVisualWidget::reset()
{
    memset(&mSamleValues, 0, sizeof(float)*LEVEL_COUNT);
    memset(&mTopBlockValue, 0, sizeof(int)*LEVEL_COUNT);
    memset(&mTopBlockSpeed, 0, sizeof(int)*LEVEL_COUNT);
    memset(&mTopBlockBlank, 0, sizeof(int)*LEVEL_COUNT);
    update();
}

bool TVisualWidget::spectrumMode()
{
    return mVisualType==VT_SPECTRUM_BLOCK || mVisualType==VT_SPECTRUM_LINE;
}

void TVisualWidget::setSwitchOnClick(bool enable)
{
    mSwitchOnClick = enable;
}

TVisualType TVisualWidget::visualType()
{
    return mVisualType;
}

void TVisualWidget::caculateTiles()
{
    int mainHeight = rect().height();
    if(mVisualType==VT_WAVE_IMPULSE || mVisualType==VT_WAVE_OSCILLOGRAM)
    {
        int halfHeight = mainHeight/2;
        for(int i=0;i<mSampleCount;i++)
        {
            float realHeight = mSamleValues[i]*halfHeight;
            if(realHeight < -halfHeight)
                realHeight = -halfHeight;
            else if (realHeight > halfHeight)
                realHeight = halfHeight;
            mSamleValues[i] = realHeight;
        }
    } else if(mVisualType == VT_SPECTRUM_BLOCK) {
        int bandWidth = mSampleCount / mBandCount;
        if(bandWidth < 4)
            return;

        float curLevel = 0;
        int index = 0;
        for(int i = 0; i < mSampleCount; i++)
        {
            curLevel += mSamleValues[i];
            if((i+1)%bandWidth == 0)
            {
                mSamleValues[index++] = curLevel * mainHeight / bandWidth;
                curLevel = 0;
            }
        }
        mSampleCount = mBandCount;
    } else {
        for(int i=0;i<mSampleCount;i++)
        {
            float realHeight = mSamleValues[i]*mainHeight;
            if(realHeight<0 && (mVisualType!=VT_WAVE_IMPULSE || mVisualType!=VT_WAVE_OSCILLOGRAM))
                realHeight = 0;
            else if (realHeight > mainHeight)
                realHeight = mainHeight;
            mSamleValues[i] = realHeight;
        }
    }
    update();
}

void TVisualWidget::drawSpectrumBlock(QPainter *painter, QRect mainRect, QBrush pillarBrush)
{
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();
    int nWidth = mSpectrumWidth + mSpectrumSpace;

    QBrush blockBrush(mColorBlock);

    int offsetX = 0;
    for(int i=0;i<mBandCount;i++)
    {
        if(offsetX+mSpectrumWidth>mainWidth)
            break;

        int realSpecValue = mSamleValues[i];

        // draw spectrum pillar
        if(realSpecValue > 0)
            painter->fillRect(offsetX, mainHeight-realSpecValue, mSpectrumWidth, realSpecValue, pillarBrush);

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
        if(topBlockValue > mainHeight-mSpectrumTopBlockHeight)
            topBlockValue = mainHeight-mSpectrumTopBlockHeight;
        // fill block
        painter->fillRect(offsetX, mainHeight-topBlockValue-mSpectrumTopBlockHeight, mSpectrumWidth, mSpectrumTopBlockHeight, blockBrush);

        mTopBlockSpeed[i] = topBlockSpeed;
        mTopBlockValue[i] = topBlockValue;
        offsetX += nWidth;
    }
}

void TVisualWidget::drawSpectrumLine(QPainter *painter, QRect mainRect, QBrush pillarBrush)
{
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();
    int bandWidth = mSampleCount / mainWidth;

    int mapLevel[mainWidth];
    for(int i=0;i<mainWidth;i++)
    {
        mapLevel[i] = (i+1)*bandWidth;
    }
    int mapIndex = 0;
    for(int i=0;i<mainWidth;i++)
    {
        float specValue = 0;
        int boundLimit = qMin(mapLevel[i], mSampleCount);

        int nSpecValues = 0;
        for(int j=mapIndex;j<boundLimit;j++)
        {
            specValue += mSamleValues[mapIndex++];
            nSpecValues++;
        }

        int realSpecValue = specValue / nSpecValues;

        // draw spectrum pillar
        if(realSpecValue > 0)
            painter->fillRect(i, mainHeight-realSpecValue, 1, realSpecValue, pillarBrush);
    }
}

void TVisualWidget::drawWaveImpulse(QPainter *painter, QRect mainRect, QBrush pillarBrush)
{
    int mainWidth = mainRect.width();
    int mainHeight = mainRect.height();
    int bandWidth = mSampleCount / mainWidth;

    int mapLevel[mainWidth];
    for(int i=0;i<mainWidth;i++)
    {
        mapLevel[i] = (i+1)*bandWidth;
    }
    int mapIndex = 0;
    int halfHeight = mainHeight/2;

    for(int i=0;i<mainWidth;i++)
    {
        int boundLimit = qMin(mapLevel[i], mSampleCount);

        int nPositive = 0;
        int nNegative = 0;
        float nPositiveValue = 0;
        float nNegativeValue = 0;
        for(int j=mapIndex;j<boundLimit;j++)
        {
            float t = mSamleValues[mapIndex++];
            if(t > 0)
            {
                nPositiveValue += t;
                nPositive++;
            } else if(t < 0){
                nNegativeValue += t;
                nNegative++;
            }
        }

        int realPositiveValue = nPositiveValue / nPositive;
        int realNegativeValue = -nNegativeValue / nNegative;
        // draw spectrum pillar
        if(realPositiveValue > 0)
            painter->fillRect(i, halfHeight-realPositiveValue, 1, realPositiveValue, pillarBrush);
        if(realNegativeValue > 0)
            painter->fillRect(i, halfHeight, 1, realNegativeValue, pillarBrush);
    }
}

void TVisualWidget::drawWaveOsci(QPainter *painter, QRect mainRect, QBrush pillarBrush)
{
    Q_UNUSED(pillarBrush);
    painter->drawText(0, mainRect.height()/2, "Not implemented!");
}

void TVisualWidget::drawVideo(QPainter *painter, QRect mainRect, QBrush pillarBrush)
{
    Q_UNUSED(pillarBrush);
    painter->drawText(0, mainRect.height()/2, "Not implemented!");
}

void TVisualWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QRect mainRect = rect();

    painter.fillRect(mainRect, QBrush());

    QLinearGradient linear(0, mSpectrumWidth/2, 0, height());
    linear.setColorAt(0, mColorTop);
    if(mColorMiddle.isValid())
        linear.setColorAt(0.5, mColorMiddle);
    linear.setColorAt(1, mColorBottom);

    QBrush pillarBrush(linear);

    switch (mVisualType) {
    case VT_WAVE_IMPULSE:
        drawWaveImpulse(&painter, mainRect, pillarBrush);
        break;
    case VT_WAVE_OSCILLOGRAM:
        drawWaveOsci(&painter, mainRect, pillarBrush);
        break;
    case VT_SPECTRUM_LINE:
        drawSpectrumLine(&painter, mainRect, pillarBrush);
        break;
    case VT_VIDEO:
        drawVideo(&painter, mainRect, pillarBrush);
        break;
    case VT_SPECTRUM_BLOCK:
        drawSpectrumBlock(&painter, mainRect, pillarBrush);
        break;
    default:
        break;
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

    if(mSwitchOnClick && event->button()==Qt::LeftButton)
    {
        mVisualType = (TVisualType)((int)mVisualType+1);
        if(mVisualType == VT_WAVE_OSCILLOGRAM)
            mVisualType = VT_SPECTRUM_BLOCK;
        if(mVisualType == VisualTypeCount)
            mVisualType = (TVisualType)0;
    }
}

void TVisualWidget::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    Q_UNUSED(skin)

    setGeometry(SkinUtils::extractGeometry(element));
    mColorBlock = QColor(element.attribute(ATTR_BLOCK_COLOR, DEFAULT_COLOR));
    mColorTop = QColor(element.attribute(ATTR_TOP_COLOR, DEFAULT_COLOR));
    mColorMiddle = QColor(element.attribute(ATTR_MIDDLE_COLOR, DEFAULT_COLOR));
    mColorBottom = QColor(element.attribute(ATTR_BOTTOM_COLOR, DEFAULT_COLOR));

    mBandCount = width() / (SAMPLE_WIDTH+SAMPLE_SPACE);
}
