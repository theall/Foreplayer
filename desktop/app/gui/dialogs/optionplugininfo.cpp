#include "optionplugininfo.h"
#include "ui_optionplugininfo.h"

TOptionPluginInfo::TOptionPluginInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOptionPluginInfo)
{
    ui->setupUi(this);
}

TOptionPluginInfo::~TOptionPluginInfo()
{
    delete ui;
}
