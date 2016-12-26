#include "visualwidget.h"

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

        int specValue = mSamleValues[i];

        // draw spectrum pillar
        p.fillRect(x, mainHeight-specValue, mSpectrumWidth, specValue, pillarBrush);

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
    Q_UNUSED(skin)

    setGeometry(SkinUtils::extractGeometry(element));
    mColorBlock = QColor(element.attribute(ATTR_BLOCK_COLOR));
    mColorTop = QColor(element.attribute(ATTR_BLOCK_COLOR));
    mColorMiddle = QColor(element.attribute(ATTR_BLOCK_COLOR));
    mColorBottom = QColor(element.attribute(ATTR_BLOCK_COLOR));
}
