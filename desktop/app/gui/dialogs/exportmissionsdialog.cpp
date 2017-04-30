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
#include "exportmissionsdialog.h"
#include "ui_exportmissionsdialog.h"

TExportItemDelegate::TExportItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void TExportItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1) {
        float value = index.model()->data(index).toFloat() * 100;
        QStyleOptionProgressBarV2 pbOption;
        pbOption.rect = option.rect.adjusted(4, 4, -4, -4);
        pbOption.minimum = 0;
        pbOption.maximum = 100;
        pbOption.textAlignment = Qt::AlignRight;
        pbOption.textVisible = true;
        pbOption.progress = value;
        pbOption.text = tr("%1%").arg(pbOption.progress);

        painter->save();
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setBrush(option.palette.highlightedText());
        }
        qApp->style()->drawControl(QStyle::CE_ProgressBar, &pbOption, painter);
        painter->restore();
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

TExportMissionsDialog::TExportMissionsDialog(QWidget *parent) :
    TAbstractDialog(parent, true),
    ui(new Ui::TExportMissionsDialog)
{
    ui->setupUi(this);

    ui->tvMissions->setFrameShape(QFrame::Panel);
    ui->tvMissions->setFrameShadow(QFrame::Sunken);
    ui->tvMissions->setShowGrid(false);
    ui->tvMissions->verticalHeader()->setVisible(false);
    ui->tvMissions->verticalHeader()->setDefaultSectionSize(20);
    ui->tvMissions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvMissions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvMissions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tvMissions->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tvMissions->setMouseTracking(true);
    ui->tvMissions->setItemDelegate(new TExportItemDelegate(this));
    ui->tvMissions->horizontalHeader()->setStretchLastSection(true);
    ui->tvMissions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    retranslateUi();
}

TExportMissionsDialog::~TExportMissionsDialog()
{
    delete ui;
}

void TExportMissionsDialog::setModel(QAbstractTableModel *model)
{
    ui->tvMissions->setModel(model);
    ui->tvMissions->resizeColumnsToContents();
}

void TExportMissionsDialog::on_btnSetting_clicked()
{
    hide();

    emit requestCloseWindow();
}

void TExportMissionsDialog::retranslateUi()
{
    ui->retranslateUi(this);
}
