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
#ifndef TEQUALIZERWINDOW_H
#define TEQUALIZERWINDOW_H

#include "widgets/imagebutton.h"
#include "widgets/sliderbar.h"
#include "abstractwindow.h"

#define EQ_FACTORS 10

class TEqualizerWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TEqualizerWindow(QWidget *parent=0);

signals:
    void eqWindowToggled(bool);
    void eqEnableToggled(bool);
    // Value is between 0.0~1.0f, center value is 0.5f
    void eqBalanceChanged(float value);
    void eqSurroundChanged(float);
    void eqPrempChanged(float);
    void eqFactorChanged(int index, float value);

    /**
     * @brief eqSpectrumChanged
     * @param index start from 0,end to EQ_FACTORS-1
     */
    void eqSpectrumChanged(int index, int value);

private slots:
    void on_btnClose_clicked();
    void on_btnReset_clicked();
    void on_btnEnabled_clicked();
    void on_eqFactor_valueChanged(int value);
    void on_balance_valueChanged(int value);
    void on_surround_valueChanged(int value);
    void on_preamp_valueChanged(int value);

private:
    TImageButton *mBtnClose;
    TImageButton *mBtnEnabled;
    TImageButton *mBtnProfile;
    TImageButton *mBtnReset;
    TSliderBar *mSldBalance;
    TSliderBar *mSldSurround;
    TSliderBar *mSldPreamp;
    QList<TSliderBar *> mSldEqFactors;
    void retranslateUi();
    void updateFactorToolTip(int index);
    void updateBallanceTooltip();
    void updateSurroundTooltip();
    void updatePreampTooltip();
    QString dbStatusString(int value);

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TEQUALIZERWINDOW_H
