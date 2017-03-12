#ifndef OPTIONSKIN_H
#define OPTIONSKIN_H

#include <QWidget>

namespace Ui {
class TOptionSkin;
}

class TOptionSkin : public QWidget
{
    Q_OBJECT

public:
    explicit TOptionSkin(QWidget *parent = 0);
    ~TOptionSkin();

private:
    Ui::TOptionSkin *ui;
};

#endif // OPTIONSKIN_H
