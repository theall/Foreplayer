#include "optiongeneral.h"
#include "ui_optiongeneral.h"

TOptionGeneral::TOptionGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOptionGeneral)
{
    ui->setupUi(this);
}

TOptionGeneral::~TOptionGeneral()
{
    delete ui;
}

void TOptionGeneral::on_comboBox_currentIndexChanged(int index)
{

}
