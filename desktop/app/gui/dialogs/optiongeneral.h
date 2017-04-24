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
#ifndef OPTIONGENERAL_H
#define OPTIONGENERAL_H

#include "optionpagebase.h"

namespace Ui {
class TOptionGeneral;
}

class TOptionGeneral : public TOptionPageBase
{
    Q_OBJECT

public:
    explicit TOptionGeneral(QWidget *parent = 0);
    ~TOptionGeneral();

signals:
    void displayTrayIconToggled(bool);

private slots:
    void on_cmbLanguage_currentIndexChanged(int index);
    void on_ckbAutoPlay_toggled(bool checked);
    void on_ckbTrayIcon_toggled(bool checked);
    void on_ckbMultiInstance_toggled(bool checked);
    void on_rbDisable_toggled(bool checked);
    void on_rbAuto_toggled(bool checked);
    void on_rbForce_toggled(bool checked);
    void on_sbCheckDuration_valueChanged(int arg1);
    void on_tePlayDuration_timeChanged(const QTime &time);

private:
    Ui::TOptionGeneral *ui;

    // TOptionPageBase interface
public:
    void retranslateUi() Q_DECL_OVERRIDE;
};

#endif // OPTIONGENERAL_H
