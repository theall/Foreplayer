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

TSplitter::TSplitter(QWidget *parent) :
    QSplitter(parent)
{

}

QSplitterHandle *TSplitter::createHandle()
{
    return new TSplitterHandle(this);
}
