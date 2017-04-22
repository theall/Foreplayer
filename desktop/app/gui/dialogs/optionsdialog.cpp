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

#include "optionplugininfo.h"
#include "optiongeneral.h"
#include "optionabout.h"
#include "optionskin.h"

#include "preferences.h"

const char *g_pageTitle[PI_COUNT] = {
    "General",
    "Plugins",
    "Skin",
    "About"
};

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TOptionsDialog)
{
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel(this);
    QStringList pageTitles;
    for(int i=0;i<PI_COUNT;i++)
    {
        pageTitles.append(tr(g_pageTitle[i]));
    }
    model->setStringList(pageTitles);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this,
            SLOT(slotCurrentRowChanged(QModelIndex, QModelIndex)));

    TOptionGeneral *optionGeneral = new TOptionGeneral;
    connect(optionGeneral, SIGNAL(displayTrayIconToggled(bool)), this, SIGNAL(displayTrayIconToggled(bool)));

    mPages.append(optionGeneral);
    mPages.append(new TOptionPluginInfo);
    mPages.append(new TOptionSkin);
    mPages.append(new TOptionAbout);

    retranslateUi();
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}

void TOptionsDialog::openPage(PageIndex pageIndex)
{
    locatePage(pageIndex);

    exec();
}

void TOptionsDialog::locatePage(PageIndex pageIndex)
{
    for(QWidget *page : mPages)
        page->hide();

    mPages[pageIndex]->setParent(ui->pageContent);
    mPages[pageIndex]->showNormal();

    ui->listView->clearMask();
    ui->listView->clearSelection();
    ui->listView->selectionModel()->select(ui->listView->model()->index((int)pageIndex, 0), QItemSelectionModel::Select);
}

void TOptionsDialog::retranslateUi()
{
    ui->retranslateUi(this);
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
