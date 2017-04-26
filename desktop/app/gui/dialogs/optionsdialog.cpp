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
#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QStringListModel>

#include "optionabout.h"

#include "preferences.h"

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    TAbstractDialog(parent)
  , ui(new Ui::TOptionsDialog)
  , mLastPage(NULL)
{
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel(this);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(slotCurrentRowChanged(QModelIndex, QModelIndex)));

    mPages.append(new TOptionGeneral);
    mPages.append(new TOptionPluginInfo);
    mPages.append(new TOptionSkin);
    mPages.append(new TOptionAbout);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->setMargin(3);
    ui->pageContent->setLayout(boxLayout);

    retranslateUi();
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}

void TOptionsDialog::openPage(PageIndex pageIndex)
{
    ui->listView->selectionModel()->setCurrentIndex(ui->listView->model()->index((int)pageIndex, 0), QItemSelectionModel::Select);

    exec();
}

TOptionGeneral *TOptionsDialog::optionGeneral()
{
    return (TOptionGeneral*)mPages[PI_GENERAL];
}

TOptionSkin *TOptionsDialog::optionSkin()
{
    return (TOptionSkin*)mPages[PI_SKIN];
}

TOptionPluginInfo *TOptionsDialog::optionPluginInfo()
{
    return (TOptionPluginInfo*)mPages[PI_PLUGIN];
}

void TOptionsDialog::locatePage(PageIndex pageIndex)
{
    for(QWidget *page : mPages)
        page->hide();

    if(mLastPage)
        ui->pageContent->layout()->removeWidget(mLastPage);

    mLastPage = mPages[pageIndex];
    ui->pageContent->layout()->addWidget(mLastPage);
    mLastPage->showNormal();

    ui->listView->clearMask();
    ui->listView->clearSelection();
    ui->listView->selectionModel()->select(ui->listView->model()->index((int)pageIndex, 0), QItemSelectionModel::Select);
}

void TOptionsDialog::retranslateUi()
{
    ui->retranslateUi(this);

    QStringList pageTitles;
    pageTitles.append(tr("General"));
    pageTitles.append(tr("Plugins"));
    pageTitles.append(tr("Skin"));
    pageTitles.append(tr("About"));
    ((QStringListModel*)ui->listView->model())->setStringList(pageTitles);
}

void TOptionsDialog::slotCurrentRowChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);

    locatePage(PageIndex(current.row()));
}

void TOptionsDialog::on_btnClose_clicked()
{
    TPreferences::instance()->save();

    close();
}
