#include "optionabout.h"
#include "ui_optionabout.h"

TOptionAbout::TOptionAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOptionAbout)
{
    ui->setupUi(this);
}

TOptionAbout::~TOptionAbout()
{
    delete ui;
}
