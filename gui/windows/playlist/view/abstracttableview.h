#ifndef TABSTRACTTABLEVIEW_H
#define TABSTRACTTABLEVIEW_H

#include "../../widgets/scrollbar.h"

class TTableViewDelegate : public QStyledItemDelegate
{
public:
    explicit TTableViewDelegate(QObject *parent = 0);

    void setPixmap(QPixmap pixmap);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QPixmap mSelectedPixmap;
};

class TAbstractTableView : public QTableView
{
    Q_OBJECT

public:
    TAbstractTableView(QWidget *parent = 0);

    static void setBackgroundColor(QColor color);
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:
    void onCurrentRowSelected(int index);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    virtual void updateColumnsWidth() = 0;

private slots:

private:
    static QColor mBackground;
};

#endif // TABSTRACTTABLEVIEW_H
