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
#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include "abstractdialog.h"

namespace Ui {
class TPropertyDialog;
}

class TPropertyDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TPropertyDialog(QWidget *parent = 0);
    ~TPropertyDialog();

    void setMusicFile(QString fileName);
    void setTitle(QString title);
    void setAuthor(QString author);
    void setYear(QString year);
    void setSystem(QString system);
    void setAddionalInfo(QString other);
    void setIndex(int current, int total);

signals:
    void onPreviousClicked();
    void onNextClicked();

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnClose_clicked();

private:
    Ui::TPropertyDialog *ui;

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // PROPERTYDIALOG_H
