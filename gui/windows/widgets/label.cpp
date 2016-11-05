#include "label.h"

TLabel::TLabel(QWidget *parent):
    QLabel(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void TLabel::setFontColor(QFont font, QColor color)
{
    setFont(font);
    QPalette pa;
    pa.setColor(QPalette::WindowText, color);
    setPalette(pa);
}

