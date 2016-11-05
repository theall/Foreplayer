#include "splitter.h"

TSplitterHandle::TSplitterHandle(QSplitter *parent) :
    QSplitterHandle(Qt::Horizontal, parent),
    mBtnExpand(new TImageButton(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(mBtnExpand);
}

void TSplitterHandle::setPixmap(QPixmap pixmap)
{
    mBtnExpand->setPixmap(pixmap);
}

void TSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QLinearGradient gradient;
    if (orientation() == Qt::Horizontal) {
        gradient.setStart(rect().left(), rect().height()/2);
        gradient.setFinalStop(rect().right(), rect().height()/2);
    } else {
        gradient.setStart(rect().width()/2, rect().top());
        gradient.setFinalStop(rect().width()/2, rect().bottom());
    }
    painter.fillRect(event->rect(), QBrush(gradient));
}

// ---------------------------------------------------------------------------------
TSplitter::TSplitter(QWidget *parent) :
    QSplitter(Qt::Horizontal, parent)
{
    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    sizePolicy->setHorizontalStretch(90);
    sizePolicy->setVerticalStretch(0);
    sizePolicy->setHeightForWidth(false);
    setSizePolicy(*sizePolicy);
    setMaximumSize(QSize(16777215, 16777215));
    setChildrenCollapsible(true);
}

QSplitterHandle *TSplitter::createHandle()
{
    return new TSplitterHandle(this);
}
