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

#include <QTimer>

void configTableView(QTableView *tableView)
{
    if(tableView)
    {
        tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
        tableView->verticalHeader()->setVisible(false);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

TOptionPluginInfo::TOptionPluginInfo(QWidget *parent) :
    TOptionPageBase(parent),
    ui(new Ui::TOptionPluginInfo)
{
    ui->setupUi(this);

    configTableView(ui->tvPlugins);
    configTableView(ui->tvSuffix);

    ui->tvPlugins->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tvSuffix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

TOptionPluginInfo::~TOptionPluginInfo()
{
    delete ui;
}

void TOptionPluginInfo::setPluginModel(QAbstractItemModel *model)
{
    ui->tvPlugins->setModel(model);

    ui->tvPlugins->resizeColumnsToContents();

    connect(ui->tvPlugins->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(slotCurrentPluginRowChanged(QModelIndex,QModelIndex)));

    ui->tvPlugins->selectionModel()->select(model->index(0, 0), QItemSelectionModel::Select);
}

void TOptionPluginInfo::setSuffixModel(QAbstractItemModel *model)
{
    ui->tvSuffix->setModel(model);

    ui->tvSuffix->resizeColumnsToContents();
}

void TOptionPluginInfo::slotCurrentPluginRowChanged(QModelIndex newIndex, QModelIndex oldIndex)
{
    Q_UNUSED(oldIndex);

    if(newIndex.isValid())
        emit currentPluginIndexChanged(newIndex.row());
}

void TOptionPluginInfo::retranslateUi()
{
    ui->retranslateUi(this);
}
