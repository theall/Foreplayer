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
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include "abstractdialog.h"

#include "optiongeneral.h"
#include "optionskin.h"

#include <QItemSelectionModel>

namespace Ui {
class TOptionsDialog;
}

enum PageIndex
{
    PI_GENERAL,
    PI_PLUGIN,
    PI_SKIN,
    PI_ABOUT,

    PI_COUNT
};

class TOptionsDialog : public TAbstractDialog
{
    Q_OBJECT

public:
    explicit TOptionsDialog(QWidget *parent = 0);
    ~TOptionsDialog();

    void openPage(PageIndex pageIndex);
    TOptionGeneral *optionGeneral();
    TOptionSkin *optionSkin();

private:
    Ui::TOptionsDialog *ui;
    QList<QWidget*> mPages;
    QWidget *mLastPage;
    void locatePage(PageIndex pageIndex);

    // TAbstractDialog interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;

private slots:
    void slotCurrentRowChanged(QModelIndex current, QModelIndex previous);
    void on_btnClose_clicked();
};

#endif // OPTIONSDIALOG_H
