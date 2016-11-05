#ifndef TLINKLABEL_H
#define TLINKLABEL_H

#include "label.h"

class TLinkLabel : public TLabel
{
public:
    TLinkLabel(QWidget *parent=0);
    void setFontColor(QFont font, QColor color) Q_DECL_OVERRIDE;

};

#endif // TLINKLABEL_H
