#ifndef TSPLITTER_H
#define TSPLITTER_H

#include "pch.h"
#include "timagebutton.h"

class TSplitterHandle : public QSplitterHandle
{
public:
    TSplitterHandle(QSplitter *parent = 0);

    void setPixmap(QPixmap pixmap);

private:
    TImageButton *mBtnExpand;
};

class TSplitter : public QSplitter
{
public:
    TSplitter(QWidget *parent = 0);

    // QSplitter interface
protected:
    QSplitterHandle *createHandle() Q_DECL_OVERRIDE;
};



#endif // TSPLITTER_H
