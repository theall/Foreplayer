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
#include "propertydialog.h"
#include "ui_propertydialog.h"

TPropertyDialog::TPropertyDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TPropertyDialog)
{
    ui->setupUi(this);

    retranslateUi();
}

TPropertyDialog::~TPropertyDialog()
{
    delete ui;
}
void TPropertyDialog::setMusicFile(QString fileName)
{
     ui->leMusicFile->setText(fileName);
}

void TPropertyDialog::setTitle(QString title)
{
     ui->leTitle->setText(title);
}

void TPropertyDialog::setAuthor(QString author)
{
     ui->leAuthor->setText(author);
}

void TPropertyDialog::setYear(QString year)
{
     ui->leYear->setText(year);
}

void TPropertyDialog::setSystem(QString system)
{
     ui->leSystem->setText(system);
}

void TPropertyDialog::setAddionalInfo(QString other)
{
    ui->txtOther->setPlainText(other);
}

void TPropertyDialog::setIndex(int current, int total)
{
    QString title = windowTitle();
    int pos = title.indexOf("(");
    if(pos != -1)
        title.truncate(pos);
    title.append("(%1/%2)");
    setWindowTitle(title.arg(current).arg(total));
    ui->btnPrevious->setEnabled(current>1&&current<=total);
    ui->btnNext->setEnabled(current>0&&current<total);
}

void TPropertyDialog::on_btnNext_clicked()
{
    emit onNextClicked();
}

void TPropertyDialog::on_btnPrevious_clicked()
{
    emit onPreviousClicked();
}

void TPropertyDialog::on_btnClose_clicked()
{
    hide();
}

void TPropertyDialog::retranslateUi()
{
    ui->retranslateUi(this);

}
