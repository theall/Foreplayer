#include "tsplitter.h"

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
    QSplitter(parent)
{

}

QSplitterHandle *TSplitter::createHandle()
{
    return new TSplitterHandle(this);
}
