#include "abstractmodel.h"

QFont TAbstractModel::mFont;
QColor TAbstractModel::mBackColor1;
QColor TAbstractModel::mBackColor2;
QColor TAbstractModel::mTextColor;
QColor TAbstractModel::mCurrentRowTextColor;

TAbstractModel::TAbstractModel(QObject *parent) :
    QAbstractTableModel(parent),
    mCurrentRow(-1)
{
}

QVariant TAbstractModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::BackgroundRole)
    {
        int row = index.row();
        if(row%2==1)
            return TAbstractModel::mBackColor2;

        return TAbstractModel::mBackColor1;
    } else if (role==Qt::FontRole) {
        return TAbstractModel::mFont;
    } else if (role==Qt::TextColorRole) {
        if(index.row()==mCurrentRow)
            return mCurrentRowTextColor;
        return TAbstractModel::mTextColor;
    }

    return QVariant();
}

void TAbstractModel::setCurrentIndex(int index)
{
    mCurrentRow = index;
}

void TAbstractModel::setFont(QFont font)
{
    mFont = font;
}

void TAbstractModel::setBackgroundColor(QColor color)
{
    mBackColor1 = color;
    mBackColor2 = color;
    mBackColor1.setAlpha(0);
    mBackColor2.setAlpha(64);
}

void TAbstractModel::setTextColor(QColor color)
{
    mTextColor = color;
}

void TAbstractModel::setCurrentRowTextColor(QColor color)
{
    mCurrentRowTextColor = color;
}
