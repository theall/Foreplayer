#ifndef TLABEL_H
#define TLABEL_H

#include "utils.h"
#include "../../share/skin.h"

class TLabel : public QLabel, TSkinReader
{
    Q_OBJECT

public:
    TLabel(QWidget *parent=0);

    virtual void setFontColor(QFont font, QColor color);

signals:
    void clicked();

private:
    // TSkinReader interface

public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // TLABEL_H
