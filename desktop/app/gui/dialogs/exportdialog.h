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
#ifndef TEXPORTDIALOG_H
#define TEXPORTDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TExportDialog;
}

class TExportDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TExportDialog(QWidget *parent = 0);
    ~TExportDialog();

    void setMusicFile(QString fileName);
    void setIndexInfo(QString indexName);
    void setIndexInfo(QStringList indexList);
    void setMaxDuration(int microSeconds);
    void setOutputPath(QString fileName);

    QString getMusicFileName();
    QString getOutputDir();
    QStringList getIndexInfo();
    QString getFormat();
    int getMaxDuration();
    bool overWrite();
    bool autoCorrect();

signals:
    void onAddClicked();

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_btnChooseDir_clicked();

private:
    Ui::TExportDialog *ui;

    QStringList mIndexList;

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // TEXPORTDIALOG_H
