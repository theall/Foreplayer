#include "optionskin.h"
#include "ui_optionskin.h"

TOptionSkin::TOptionSkin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOptionSkin)
{
    ui->setupUi(this);
}

TOptionSkin::~TOptionSkin()
{
    delete ui;
}
