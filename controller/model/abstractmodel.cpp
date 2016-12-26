#include "abstractmodel.h"

QVariant TAbstractModel::mFont;
QVariant TAbstractModel::mBackColor1;
QVariant TAbstractModel::mBackColor2;
QVariant TAbstractModel::mTextColor;
QVariant TAbstractModel::mCurrentRowTextColor;
QVariant TAbstractModel::mSelectedTextColor;
QVariant TAbstractModel::mNumberColor;
QVariant TAbstractModel::mDurationColor;

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
    } else if (role==Utils::TextHighlight) {
        return TAbstractModel::mSelectedTextColor;
    } else if (role==Utils::IsCurrentRow) {
        return index.row()==mCurrentRow;
    }

    return QVariant();
}

void TAbstractModel::setCurrentIndex(int index)
{
    mCurrentRow = index;

    QVector<int> roles;
    roles.append(Qt::TextColorRole);
    emit dataChanged(QModelIndex(), QModelIndex(), roles);
}

void TAbstractModel::setPlaylistCore(TPlaylistCore *core)
{
    mPlaylistCore = core;
}

void TAbstractModel::setFont(QFont font)
{
    mFont = font;
}

void TAbstractModel::setBackgroundColor(QColor color)
{
    color.setAlpha(0);
    mBackColor1 = color;
    color.setAlpha(64);
    mBackColor2 = color;
}

void TAbstractModel::setTextColor(QColor color)
{
    mTextColor = color;
}

void TAbstractModel::setCurrentRowTextColor(QColor color)
{
    mCurrentRowTextColor = color;
}

void TAbstractModel::setSelectedTextColor(QColor color)
{
    mSelectedTextColor = color;
}

void TAbstractModel::setColorParameters(QFont font,
                                        QColor textColor,
                                        QColor selectedTextColor,
                                        QColor numberColor,
                                        QColor durationColor,
                                        QColor currentRowTextColor,
                                        QColor backgroundColor)
{
    mFont = font;
    backgroundColor.setAlpha(0);
    mBackColor1 = backgroundColor;
    backgroundColor.setAlpha(64);
    mBackColor2 = backgroundColor;
    mTextColor = textColor;
    mCurrentRowTextColor = currentRowTextColor;
    mSelectedTextColor = selectedTextColor;
    mNumberColor = numberColor;
    mDurationColor = durationColor;
}
