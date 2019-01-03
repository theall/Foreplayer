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
#include "optionabout.h"
#include "ui_optionabout.h"

#include <QDesktopServices>

TOptionAbout::TOptionAbout(QWidget *parent) :
    TOptionPageBase(parent),
    ui(new Ui::TOptionAbout)
{
    ui->setupUi(this);

    retranslateUi();
}

TOptionAbout::~TOptionAbout()
{
    delete ui;
}

void TOptionAbout::retranslateUi()
{
    ui->retranslateUi(this);

    ui->lblVersion->setText(tr("%1(Version%2)").arg(qApp->applicationDisplayName()).arg(qApp->applicationVersion()));
    ui->lblCopyright->setText(qApp->organizationDomain());
}
