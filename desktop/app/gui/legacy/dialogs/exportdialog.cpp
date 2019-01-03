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
#include "exportdialog.h"
#include "ui_exportdialog.h"

#include <QString>
#include <QFileDialog>

TExportDialog::TExportDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TExportDialog)
{
    ui->setupUi(this);

    ui->leFileName->setReadOnly(true);
    ui->leIndexTitle->setReadOnly(true);
    ui->leMusicTitle->setReadOnly(true);
    ui->dtDuration->setDisplayFormat("HH:mm:ss.zzz");

    retranslateUi();
}

TExportDialog::~TExportDialog()
{
    delete ui;
}

void TExportDialog::setMusicTitle(QString title)
{
    ui->leMusicTitle->setText(title);
}

void TExportDialog::setMusicFile(QString fileName)
{
    ui->leFileName->setText(fileName);
}

void TExportDialog::setIndexInfo(QList<QPair<QString, QString>> indexList)
{
    mIndexInfo = indexList;
    if(indexList.size() > 1)
    {
        ui->leIndexTitle->setText(QString("%1 Tracks").arg(indexList.size()));
    } else {
        ui->leIndexTitle->setText(indexList[0].second);
    }
    ui->ckbNumber->setChecked(mIndexInfo.size()>1);
}

void TExportDialog::setMaxDuration(int microSeconds)
{
    QTime time;
    time.addMSecs(microSeconds);
    ui->dtDuration->setTime(time);
    ui->ckbCorrect->setChecked(microSeconds<=0);
}

void TExportDialog::setOutputPath(QString fileName)
{
    ui->leDir->setText(fileName);
#ifdef QT_DEBUG
        ui->leDir->setText("z:/");
#endif
}

bool TExportDialog::autoNumber()
{
    return ui->ckbNumber->isChecked();
}

QString TExportDialog::getMusicFileName()
{
    return ui->leFileName->text();
}

QString TExportDialog::getMusicTitle()
{
    return ui->leMusicTitle->text();
}

QString TExportDialog::getOutputDir()
{
    return ui->leDir->text();
}

QList<QPair<QString, QString>> TExportDialog::getIndexInfo()
{
    return mIndexInfo;
}

QString TExportDialog::getFormat()
{
    return ui->cmbFormat->currentText();
}

int TExportDialog::getMaxDuration()
{
    return ui->dtDuration->time().msecsSinceStartOfDay();
}

bool TExportDialog::overWrite()
{
    return ui->ckbOverwrite->isChecked();
}

bool TExportDialog::autoCorrect()
{
    return ui->ckbCorrect->isChecked();
}

void TExportDialog::on_btnOk_clicked()
{
    hide();
    emit onAddClicked();
}

void TExportDialog::on_btnCancel_clicked()
{
    hide();
}

void TExportDialog::on_btnChooseDir_clicked()
{
    QFileInfo fi(ui->leFileName->text());
    QString dir = QFileDialog::getExistingDirectory(
                this,
                tr("Choose directory"),
                fi.absoluteFilePath());
    if(!dir.isEmpty())
        ui->leDir->setText(dir);
}

void TExportDialog::retranslateUi()
{
    ui->retranslateUi(this);

}
