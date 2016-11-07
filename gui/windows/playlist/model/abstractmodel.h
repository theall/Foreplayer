#ifndef TABSTRACTMODEL_H
#define TABSTRACTMODEL_H

#include "utils.h"

class TAbstractModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TAbstractModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    void setCurrentIndex(int index);

    static void setFont(QFont font);
    static void setBackgroundColor(QColor color);
    static void setTextColor(QColor color);
    static void setCurrentRowTextColor(QColor color);
    static void setSelectedTextColor(QColor color);
    static void setColorParameters(QFont font,
                                   QColor textColor,
                                   QColor selectedTextColor,
                                   QColor numberColor,
                                   QColor durationColor,
                                   QColor currentRowTextColor,
                                   QColor backgroundColor);

private:
    static QVariant mFont;
    static QVariant mBackColor1;
    static QVariant mBackColor2;
    static QVariant mTextColor;
    static QVariant mCurrentRowTextColor;
    static QVariant mSelectedTextColor;
    static QVariant mNumberColor;
    static QVariant mDurationColor;
    int mCurrentRow;
};

#endif // TABSTRACTMODEL_H
