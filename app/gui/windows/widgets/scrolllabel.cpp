#include "scrolllabel.h"

const int c_timer_interval = 40;
const int c_idle_time = 3000;
const int c_idle_time_short = 1500;

TScrollLabel::TScrollLabel(QWidget *parent) :
    QWidget(parent),
    mLineWidth(0),
    mSwitchEnd(false),
    mCurrentIndex(-1),
    mIndex(-1),
    mStatus(END),
    mTimerID(-1)
{

}

void TScrollLabel::setStrings(QStringList strlist)
{
    mStrings = strlist;
    if(mTimerID != -1)
    {
        killTimer(mTimerID);
        mTimerID = -1;
    }
    if(strlist.count() > 0)
    {
        mIndex = 0;
        mCurrentIndex = 0;
        prepareNext();
        mStatus = IDLE;
        mIndex = 0;
        mIdleTime = 0;
        mTimerID = startTimer(c_timer_interval);
    }
}

void TScrollLabel::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
}

void TScrollLabel::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    if(mStatus==IDLE)
    {
        mIdleTime += c_timer_interval;
        // Switch if current display text is emplty, or idle time is time out
        if(mIdleTime>c_idle_time || currentLine().trimmed().isEmpty())
        {
            prepareNext();
            mSwitchEnd = false;
            mStatus = SWITCH;
        }
    }
    else if(mStatus==SWITCH)
    {
        mOffsetY--;
        if(mOffsetY < 0)
        {
            mCurrentIndex = mIndex;
            mSwitchEnd = true;
            mStatus = IDLE;
        }
    }
    else if(mStatus==SHUTTLE_LEFT)
    {
        mOffsetX-=1;
        if(width()-mOffsetX >= mLineWidth)
        {
            mStatus = IDLE_SHORT;
        }
    }
    else if(mStatus==IDLE_SHORT)
    {
        mIdleTime += c_timer_interval;
        if(mIdleTime > c_idle_time_short)
        {
            mIdleTime = 0;
            mStatus = SHUTTLE_RIGHT;
        }
    }
    else if(mStatus==SHUTTLE_RIGHT)
    {
        mOffsetX+=1;
        if(mOffsetX >= 0)
        {
            mOffsetX = 0;
            mStatus = BREAK;
        }
    }
    else if(mStatus==BREAK)
    {
        mIdleTime += c_timer_interval;
        if(mIdleTime > c_idle_time)
        {
            mIdleTime = 0;
            mStatus = SWITCH;
        }
    }
    update();
}

QString TScrollLabel::currentLine()
{
    if(mIndex<0 || mIndex>=mStrings.size())
        return QString();

    return mStrings[mIndex];
}

void TScrollLabel::prepareNext()
{
    if(mIndex<0 || mIndex>=mStrings.size())
        return;

    mLine1 = mStrings[mIndex];
    mIndex++;
    if(mIndex >= mStrings.size())
    {
        mIndex = 0;
    }
    mLine2 = mStrings[mIndex];
    mOffsetX = 0;
    mOffsetY = rect().height()-1;
    mIdleTime = 0;
}

void TScrollLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter p(this);

    int w = width();
    int h = height();

    if(!mSwitchEnd)
        p.drawText(mOffsetX, mOffsetY, mLine1);
    p.drawText(mOffsetX, mOffsetY+h, mLine2);

    // Line changed
    if(mCurrentIndex != mIndex)
    {
        QRectF needed;
        QRectF source(0, 0, w, h);
        needed = p.boundingRect(source, Qt::AlignLeft, mStrings[mCurrentIndex]);

        mLineWidth = (int)needed.width();
        if(mLineWidth > w)
            mStatus = SHUTTLE_LEFT;

        mCurrentIndex = mIndex;
    }

    p.end();
}

void TScrollLabel::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    Q_UNUSED(skin)

    setGeometry(SkinUtils::extractGeometry(element));
    setFontColor(SkinUtils::extractFont(element), QColor(element.attribute(ATTR_COLOR)));
}
