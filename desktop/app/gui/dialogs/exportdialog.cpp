#include "exportdialog.h"
#include "ui_exportdialog.h"

#include <QString>
#include <QFileDialog>

TExportDialog::TExportDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TExportDialog)
{
    ui->setupUi(this);

    ui->leMusicName->setReadOnly(true);
    ui->leIndex->setReadOnly(true);
    ui->dtDuration->setDisplayFormat("HH:mm:ss.zzz");

    retranslateUi();
}

TExportDialog::~TExportDialog()
{
    delete ui;
}

void TExportDialog::setMusicFile(QString fileName)
{
    ui->leMusicName->setText(fileName);
}

void TExportDialog::setIndexInfo(QString indexName)
{
    ui->leIndex->setText(indexName);

    mIndexList.clear();
    mIndexList.append(indexName);
}

void TExportDialog::setIndexInfo(QStringList indexList)
{
    mIndexList = indexList;
    if(mIndexList.size() > 1)
    {
        ui->leIndex->setText(QString("%1 Tracks").arg(mIndexList.size()));
    }
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

QString TExportDialog::getMusicFileName()
{
    return ui->leMusicName->text();
}

QString TExportDialog::getOutputDir()
{
    return ui->leDir->text();
}

QStringList TExportDialog::getIndexInfo()
{
    return mIndexList;
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
    QFileInfo fi(ui->leMusicName->text());
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
