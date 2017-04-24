/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "optionplugininfo.h"
#include "ui_optionplugininfo.h"

#include <QStandardItemModel>

TOptionPluginInfo::TOptionPluginInfo(QWidget *parent) :
    TOptionPageBase(parent),
    ui(new Ui::TOptionPluginInfo)
{
    ui->setupUi(this);

    ui->tvPlugins->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    QStandardItemModel *pluginModel = new QStandardItemModel();
    pluginModel->setColumnCount(5);
    pluginModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
    pluginModel->setHeaderData(1, Qt::Horizontal, tr("Manufacture"));
    pluginModel->setHeaderData(2, Qt::Horizontal, tr("Contact"));
    pluginModel->setHeaderData(3, Qt::Horizontal, tr("CreateDate"));
    pluginModel->setHeaderData(4, Qt::Horizontal, tr("Description"));
    ui->tvPlugins->setModel(pluginModel);

    ui->tvSuffix->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    QStandardItemModel *suffixModel = new QStandardItemModel();
    suffixModel->setColumnCount(2);
    suffixModel->setHeaderData(0, Qt::Horizontal, tr("Suffix"));
    suffixModel->setHeaderData(1, Qt::Horizontal, tr("Description"));
    ui->tvSuffix->setModel(suffixModel);

    emit requestPluginInformation(mPluginInfos);
}

TOptionPluginInfo::~TOptionPluginInfo()
{
    delete ui;

    for(TPluginInfo *p : mPluginInfos)
        delete p;

    mPluginInfos.clear();
}

void TOptionPluginInfo::retranslateUi()
{
    ui->retranslateUi(this);
}
