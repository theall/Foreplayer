#ifndef OPTIONABOUT_H
#define OPTIONABOUT_H

#include <QWidget>

namespace Ui {
class TOptionAbout;
}

class TOptionAbout : public QWidget
{
    Q_OBJECT

public:
    explicit TOptionAbout(QWidget *parent = 0);
    ~TOptionAbout();

private:
    Ui::TOptionAbout *ui;
};

#endif // OPTIONABOUT_H
