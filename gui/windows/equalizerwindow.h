#ifndef TEQUALIZERWINDOW_H
#define TEQUALIZERWINDOW_H

#include "../skinutils.h"

#include "widgets/imagebutton.h"
#include "widgets/sliderbar.h"
#include "abstractwindow.h"

#define EQ_FACTORS 10

class TEqualizerWindow : public TAbstractWindow
{
    Q_OBJECT

public:
    TEqualizerWindow(QWidget *parent=0);

    void setGuiParamter(EqualizerWindowParam *param);

signals:
    void eqWindowToggled(bool);
    void eqEnableToggled(bool);
    void eqBalanceChanged(int);
    void eqSurroundChanged(int);
    void eqPrempChanged(int);
    void eqFactorChanged(int index, int value);

    /**
     * @brief eqSpectrumChanged
     * @param index start from 0,end to EQ_FACTORS-1
     */
    void eqSpectrumChanged(int index, int value);

private slots:
    void on_btnClose_clicked();
    void on_btnReset_clicked();
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
};

#endif // TEQUALIZERWINDOW_H
