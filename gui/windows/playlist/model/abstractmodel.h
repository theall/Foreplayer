#ifndef TABSTRACTMODEL_H
#define TABSTRACTMODEL_H

#include "pch.h"

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

private:
    static QFont mFont;
    static QColor mBackColor1;
    static QColor mBackColor2;
    static QColor mTextColor;
    static QColor mCurrentRowTextColor;
    int mCurrentRow;
};

#endif // TABSTRACTMODEL_H
