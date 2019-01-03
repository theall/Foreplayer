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
#include "equalizerwindow.h"

#include "preferences.h"

#define EQ_FACTORS 10

const int PROFILE_ROCK[EQ_FACTORS]     = { -2, 0, 2, 4, -2, -2, 0, 0, 4, 4 };
const int PROFILE_METAL[EQ_FACTORS]    = { -6, 0, 0, 0, 0, 0, 4, 0, 4, 0 };
const int PROFILE_POP[EQ_FACTORS]      = { 3, 1, 0, -2, -4, -4, -2, 0, 1, 2 };
const int PROFILE_CLASSIC[EQ_FACTORS]  = { 0, 8, 8, 4, 0, 0, 0, 0, 2, 2 };
const int PROFILE_JAZZ[EQ_FACTORS]     = { 0, 0, 0, 4, 4, 4, 0, 2, 3, 4 };
const int PROFILE_ELECTRIC[EQ_FACTORS] = { -6, 1, 4, -2, -2, -4, 0, 0, 6, 6 };

TEqualizerWindow::TEqualizerWindow(QWidget *parent) :
    TAbstractWindow(parent)
  , mBtnClose(new TImageButton(this))
  , mBtnEnabled(new TImageButton(this))
  , mBtnProfile(new TImageButton(this))
  , mBtnReset(new TImageButton(this))
  , mSldBalance(new TSliderBar(Qt::Horizontal, this))
  , mSldSurround(new TSliderBar(Qt::Horizontal, this))
  , mSldPreamp(new TSliderBar(Qt::Vertical, this))
  , mContextMenu(new TEqualizerMenu(parent))
  , mTriggerIgnored(false)
{
    setObjectName("EqualizerWindow");

    mBtnEnabled->setCheckable(true);
    mBtnEnabled->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_E));

    mBtnProfile->setCursor(Qt::PointingHandCursor);

    mSldBalance->setRange(-10, 10);
    mSldBalance->setValue(0);

    mSldSurround->setRange(0, 16);
    mSldSurround->setValue(0);

    mSldPreamp->setRange(-12, 12);
    mSldPreamp->setValue(0);

    for(int i=0;i<EQ_FACTORS;i++)
    {
        TSliderBar *bar = new TSliderBar(Qt::Vertical, this);
        bar->setRange(-12, 12);
        bar->setValue(0);
        connect(bar, SIGNAL(valueChanged(int)), this, SLOT(on_eqFactor_valueChanged(int)));

        mSldEqFactors.append(bar);
    }

    connect(mBtnClose, SIGNAL(clicked()), this, SLOT(on_btnClose_clicked()));
    connect(mBtnReset, SIGNAL(clicked()), this, SLOT(on_btnReset_clicked()));
    connect(mBtnEnabled, SIGNAL(clicked()), this, SLOT(on_btnEnabled_clicked()));
    connect(mBtnProfile, SIGNAL(clicked()), this, SLOT(on_btnProfile_clicked()));
    connect(mSldSurround, SIGNAL(valueChanged(int)), this, SLOT(on_surround_valueChanged(int)));
    connect(mSldBalance, SIGNAL(valueChanged(int)), this, SLOT(on_balance_valueChanged(int)));
    connect(mSldPreamp, SIGNAL(valueChanged(int)), this, SLOT(on_preamp_valueChanged(int)));

    connect(mContextMenu, SIGNAL(onRockSelected()), this, SLOT(slotProfileRockSelected()));
    connect(mContextMenu, SIGNAL(onMetalSelected()), this, SLOT(slotProfileMetalSelected()));
    connect(mContextMenu, SIGNAL(onElectricSelected()), this, SLOT(slotProfileElectricSelected()));
    connect(mContextMenu, SIGNAL(onPopSelected()), this, SLOT(slotProfilePopSelected()));
    connect(mContextMenu, SIGNAL(onJazzSelected()), this, SLOT(slotProfileJazzSelected()));
    connect(mContextMenu, SIGNAL(onClassicSelected()), this, SLOT(slotProfileClassicSelected()));
    connect(mContextMenu, SIGNAL(onCustomizeSelected()), this, SLOT(slotProfileCustomizeSelected()));

    retranslateUi();
}

TEqualizerWindow::~TEqualizerWindow()
{
    TPreferences *prefs = TPreferences::instance();
    if(!prefs)
        return;

    prefs->setEqEnabled(mBtnEnabled->isChecked());
    prefs->setEqBallance(mSldBalance->value());
    prefs->setEqSurround(mSldSurround->value());
    prefs->setEqAmplification(mSldPreamp->value());

    saveEqFactors();
}

void TEqualizerWindow::loadSettings()
{
    TPreferences *prefs = TPreferences::instance();
    if(!prefs)
        return;

    mBtnEnabled->setChecked(prefs->eqEnabled());
    mBtnEnabled->clicked(prefs->eqEnabled());
    mSldBalance->setValue(prefs->eqBallance());
    mSldSurround->setValue(prefs->eqSurround());
    mSldPreamp->setValue(prefs->eqAmplification());

    mContextMenu->loadSettings();
}

void TEqualizerWindow::on_btnClose_clicked()
{
    hide();

    emit eqWindowToggled(false);
}

void TEqualizerWindow::on_btnReset_clicked()
{
    mSldPreamp->setValue(0);

    for(auto x : mSldEqFactors)
    {
        x->setValue(0);
    }
}

void TEqualizerWindow::on_btnEnabled_clicked()
{
    emit eqEnableToggled(mBtnEnabled->isChecked());
}

void TEqualizerWindow::on_btnProfile_clicked()
{
    if(mContextMenu)
        mContextMenu->popup(QCursor::pos());
}

void TEqualizerWindow::on_eqFactor_valueChanged(int value)
{
    TSliderBar *factorBar = static_cast<TSliderBar*>(sender());
    if(!factorBar)
        return;

    int i = mSldEqFactors.indexOf(factorBar);
    if(i != -1)
    {
        saveEqFactors();

        emit eqFactorChanged(i, (float)value);        
        updateFactorToolTip(i);
    }
}

void TEqualizerWindow::on_balance_valueChanged(int value)
{
    int minValue = mSldBalance->minimum();
    int maxValue = mSldBalance->maximum();
    float factor = (float)(value-minValue)/(maxValue-minValue);
    emit eqBalanceChanged(factor);

    updateBallanceTooltip();
}

void TEqualizerWindow::on_surround_valueChanged(int value)
{
    int minValue = mSldSurround->minimum();
    int maxValue = mSldSurround->maximum();
    float factor = (float)(value-minValue)/(maxValue-minValue);
    emit eqSurroundChanged(factor);

    updateSurroundTooltip();
}

void TEqualizerWindow::on_preamp_valueChanged(int value)
{
    emit eqPrempChanged((float)value);

    updatePreampTooltip();
}

void TEqualizerWindow::slotProfileRockSelected()
{
    setEqualizerFactors(PROFILE_ROCK);
}

void TEqualizerWindow::slotProfileMetalSelected()
{
    setEqualizerFactors(PROFILE_METAL);
}

void TEqualizerWindow::slotProfileElectricSelected()
{
    setEqualizerFactors(PROFILE_ELECTRIC);
}

void TEqualizerWindow::slotProfilePopSelected()
{
    setEqualizerFactors(PROFILE_POP);
}

void TEqualizerWindow::slotProfileJazzSelected()
{
    setEqualizerFactors(PROFILE_JAZZ);
}

void TEqualizerWindow::slotProfileClassicSelected()
{
    setEqualizerFactors(PROFILE_CLASSIC);
}

void TEqualizerWindow::slotProfileCustomizeSelected()
{
    QList<int> factors = TPreferences::instance()->eqFactors();
    if(mSldEqFactors.size()==factors.size())
    {
        for(int i=0;i<mSldEqFactors.size();i++)
        {
            mSldEqFactors[i]->setValue(factors[i]);
        }
    }
}

void TEqualizerWindow::setEqualizerFactors(const int values[])
{
    mTriggerIgnored = true;
    for(int i=0;i<EQ_FACTORS;i++)
    {
        mSldEqFactors[i]->setValue((float)values[i]);
    }
    mTriggerIgnored = false;
}

void TEqualizerWindow::saveEqFactors()
{
    if(!mContextMenu->isCustomizeActionChecked() && !mTriggerIgnored)
        mContextMenu->checkCustomizeAction();

    if(mContextMenu->isCustomizeActionChecked()){
        QList<int> factors;
        for(TSliderBar *sliderBar : mSldEqFactors)
            factors.append(sliderBar->value());

        TPreferences::instance()->setEqFactors(factors);
    }
}

void TEqualizerWindow::retranslateUi()
{
    mBtnEnabled->setToolTip(tr("Switch(%1)").arg(mBtnEnabled->shortcut().toString()));

    mBtnProfile->setToolTip(tr("Preset"));
    mBtnReset->setToolTip(tr("Reset"));
    mBtnClose->setToolTip(tr("Close"));

    updateBallanceTooltip();
    updateSurroundTooltip();
    updatePreampTooltip();

    for(int i=0;i<mSldEqFactors.size();i++)
    {
        updateFactorToolTip(i);
    }
}

void TEqualizerWindow::updateFactorToolTip(int index)
{
    if(index<0 || index>=mSldEqFactors.size())
        return;

    TSliderBar *factor = mSldEqFactors[index];

    factor->setToolTip(dbStatusString(factor->value()));
}

void TEqualizerWindow::updateBallanceTooltip()
{
    int value = mSldBalance->value();
    if(value==0)
        mSldBalance->setToolTip(tr("Balance: center"));
    else if(value<0)
    {
        value = -value*10;
        mSldBalance->setToolTip(tr("Left: %1%").arg(value));
    }
    else
    {
        value = value*10;
        mSldBalance->setToolTip(tr("Right: %1%").arg(value));
    }
}

void TEqualizerWindow::updateSurroundTooltip()
{
    int value = mSldSurround->value();
    if(value==0)
    {
        mSldSurround->setToolTip(tr("Surround: closed"));
    }
    else
    {
        mSldSurround->setToolTip(tr("Surround: %1").arg(value));
    }
}

void TEqualizerWindow::updatePreampTooltip()
{
    mSldPreamp->setToolTip(dbStatusString(mSldPreamp->value()));
}

QString TEqualizerWindow::dbStatusString(int value)
{
    QString prefix = "";
    if(value>0)
        prefix = "+";

    return tr("%1%2 db").arg(prefix).arg(value);
}

void TEqualizerWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(mContextMenu)
        mContextMenu->popup(event->globalPos());

    TAbstractWindow::contextMenuEvent(event);
}

void TEqualizerWindow::loadFromSkin(QDomElement element, TSkin *skin)
{
    if(!skin)
        return;

    TAbstractWindow::loadFromSkin(element, skin);

    mBtnClose->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_CLOSE), skin);
    mBtnEnabled->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_ENABLED), skin);
    mBtnProfile->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_PROFILE), skin);
    mBtnReset->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_RESET), skin);

    mSldBalance->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_BALANCE), skin);
    mSldSurround->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_SURROUND), skin);
    mSldPreamp->loadFromSkin(element.firstChildElement(TAG_EQUALIZER_PREAMP), skin);

    QDomElement factorElement = element.firstChildElement(TAG_EQUALIZER_EQFACTOR);
    QRect rt = SkinUtils::strToGeometry(factorElement.attribute(ATTR_POSITION));
    int space = element.attribute(ATTR_EQ_INTERVAL).toInt() + rt.width();
    int offsetX = rt.left();
    for(int i=0;i<EQ_FACTORS;i++)
    {
        TSliderBar *sliderBar = mSldEqFactors[i];
        sliderBar->loadFromSkin(factorElement, skin);
        rt.moveLeft(offsetX + i*space);
        sliderBar->setGeometry(rt);
    }

    update();
}
