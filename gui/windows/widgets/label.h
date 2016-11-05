#ifndef TLABEL_H
#define TLABEL_H

#include "utils.h"

class TLabel : public QLabel
{
public:
    TLabel(QWidget *parent=0);

    virtual void setFontColor(QFont font, QColor color);
};

#endif // TLABEL_H
