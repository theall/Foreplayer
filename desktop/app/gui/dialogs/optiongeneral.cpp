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
#include "optiongeneral.h"
#include "ui_optiongeneral.h"

#include "modules/languagemanager.h"
#include "preferences.h"

TOptionGeneral::TOptionGeneral(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TOptionGeneral)
{
    ui->setupUi(this);

    //Add language items
    ui->cmbLanguage->blockSignals(true);
    foreach (const QString &name, TLanguageManager::instance()->languages()) {
        QLocale locale(name);
        QString string = QString(QLatin1String("%1 (%2)"))
            .arg(QLocale::languageToString(locale.language()))
            .arg(QLocale::countryToString(locale.country()));
        ui->cmbLanguage->addItem(string, name);
    }
    ui->cmbLanguage->model()->sort(0);
    ui->cmbLanguage->insertItem(0, tr("System default"));
    ui->cmbLanguage->blockSignals(false);

    // Not found (-1) ends up at index 0, system default
    int languageIndex = ui->cmbLanguage->findData(TPreferences::instance()->language());
    if (languageIndex == -1)
        languageIndex = 0;
    ui->cmbLanguage->setCurrentIndex(languageIndex);
}

TOptionGeneral::~TOptionGeneral()
{
    delete ui;
}

void TOptionGeneral::on_cmbLanguage_currentIndexChanged(int index)
{
    const QString language = ui->cmbLanguage->itemData(index).toString();
    TPreferences::instance()->setLanguage(language);
}

void TOptionGeneral::on_ckbAutoPlay_toggled(bool checked)
{
    TPreferences::instance()->setAutoPlayAfterStarted(checked);
}

void TOptionGeneral::on_ckbTrayIcon_toggled(bool checked)
{
    TPreferences::instance()->setDisplayTrayIcon(checked);
}

void TOptionGeneral::on_ckbMultiInstance_toggled(bool checked)
{
    TPreferences::instance()->setEnableMultiInstance(checked);
}

void TOptionGeneral::on_rbDisable_toggled(bool checked)
{
    ui->sbCheckDuration->setEnabled(!checked);
    ui->tePlayDuration->setEnabled(!checked);
}

void TOptionGeneral::on_rbAuto_toggled(bool checked)
{
    TPreferences::instance()->setCorrectDuration(checked);
}

void TOptionGeneral::on_rbForce_toggled(bool checked)
{
    TPreferences::instance()->setForceCorrectDuration(checked);
}

void TOptionGeneral::on_sbCheckDuration_valueChanged(int arg1)
{
    TPreferences::instance()->setCheckDuration(arg1);
}

void TOptionGeneral::on_tePlayDuration_timeChanged(const QTime &time)
{
    QTime t(time);
    t.setHMS(0, 0, 0);
    TPreferences::instance()->setPilotDuration(time.secsTo(t));
}
