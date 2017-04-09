#ifndef OPTIONGENERAL_H
#define OPTIONGENERAL_H

#include <QWidget>

namespace Ui {
class TOptionGeneral;
}

class TOptionGeneral : public QWidget
{
    Q_OBJECT

public:
    explicit TOptionGeneral(QWidget *parent = 0);
    ~TOptionGeneral();

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::TOptionGeneral *ui;
};

#endif // OPTIONGENERAL_H
