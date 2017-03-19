#include "equalizerwindow.h"

TEqualizerWindow::TEqualizerWindow(QWidget *parent) :
    TAbstractWindow(parent),
    mBtnClose(new TImageButton(this)),
    mBtnEnabled(new TImageButton(this)),
    mBtnProfile(new TImageButton(this)),
    mBtnReset(new TImageButton(this)),
    mSldBalance(new TSliderBar(Qt::Horizontal, this)),
    mSldSurround(new TSliderBar(Qt::Horizontal, this)),
    mSldPreamp(new TSliderBar(Qt::Vertical, this))
{
    setObjectName("EqualizerWindow");

    mBtnEnabled->setCheckable(true);

    mBtnEnabled->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_E));

    mSldBalance->setRange(-10, 10);
    mSldBalance->setValue(0);

    mSldSurround->setRange(0, 16);

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
    connect(mSldSurround, SIGNAL(valueChanged(int)), this, SLOT(on_surround_valueChanged(int)));
    connect(mSldBalance, SIGNAL(valueChanged(int)), this, SLOT(on_balance_valueChanged(int)));
    connect(mSldPreamp, SIGNAL(valueChanged(int)), this, SLOT(on_preamp_valueChanged(int)));

    retranslateUi();
}

void TEqualizerWindow::on_btnClose_clicked()
{
    hide();

    emit eqWindowToggled(false);
}

void TEqualizerWindow::on_btnReset_clicked()
{
    mSldPreamp->setValue(0);;

    for(auto x : mSldEqFactors)
    {
        x->setValue(0);
    }
}

void TEqualizerWindow::on_btnEnabled_clicked()
{
    emit eqEnableToggled(mBtnEnabled->isChecked());
}

void TEqualizerWindow::on_eqFactor_valueChanged(int value)
{
    TSliderBar *factorBar = static_cast<TSliderBar*>(sender());
    if(!factorBar)
        return;

    int i = mSldEqFactors.indexOf(factorBar);
    if(i != -1)
    {
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
}
