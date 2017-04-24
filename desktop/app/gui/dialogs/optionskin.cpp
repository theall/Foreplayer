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
#include "optionskin.h"
#include "ui_optionskin.h"

#include <QStringListModel>
TOptionSkin::TOptionSkin(QWidget *parent) :
    TOptionPageBase(parent),
    ui(new Ui::TOptionSkin)
{
    ui->setupUi(this);

    ui->lvSkins->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lvSkins->setModel(new QStringListModel(ui->lvSkins));
    connect(ui->lvSkins->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(slotCurrentSkinIndexChanged(QModelIndex,QModelIndex)));
}

TOptionSkin::~TOptionSkin()
{
    delete ui;
}

void TOptionSkin::slotCurrentSkinIndexChanged(QModelIndex newIndex, QModelIndex oldIndex)
{
    Q_UNUSED(oldIndex);

    int index = newIndex.row();
    if(index >= 0)
    {
        QPixmap preview;
        QString author;
        QString webSite;
        QString contact;
        emit requestSkinInfo(index, preview, author, webSite, contact);

        ui->lblPreview->setPixmap(preview);
        ui->lblAuthor->setText(author);

        QString temp = "<html><head/><body><p><a href=\"%1\"><span style=\" text-decoration: underline; color:#0000ff;\">%1</span></a></p></body></html>";
        ui->lblWebsite->setText(temp.arg(webSite));
        ui->lblContact->setText(temp.arg(contact));
    }
}

void TOptionSkin::showEvent(QShowEvent *event)
{
    int currentIndex;
    QStringList skinNames;
    emit requestSkinNames(skinNames, currentIndex);

    QStringListModel *model = (QStringListModel*)ui->lvSkins->model();
    if(model)
    {
        model->setStringList(skinNames);
        ui->lvSkins->setCurrentIndex(model->index(currentIndex));
    }
    event->accept();
}

void TOptionSkin::on_btnApply_clicked()
{
    emit requestLoadSkin(ui->lvSkins->currentIndex().row());
}

void TOptionSkin::retranslateUi()
{
    ui->retranslateUi(this);
}
