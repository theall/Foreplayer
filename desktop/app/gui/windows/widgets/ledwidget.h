#ifndef TLEDWIDGET_H
#define TLEDWIDGET_H

#include "../../share/skin.h"

class TLedWidget : public QWidget, TSkinReader
{
    Q_OBJECT
public:
    explicit TLedWidget(QWidget *parent = 0);
    void setTime(int seconds, int total);

signals:

public slots:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    int mSeconds;
    int mTotalSeconds;
    bool mShowRemain;
    int mAlignment;
    QVector<QPixmap> mPending;
    QVector<QPixmap> mNumbers;
    QPixmap mQuote;
    QPixmap mMinus;

    void refresh();
    void setPixmap(QPixmap pixmap);
    void setAlignment(Qt::Alignment alignment);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TLEDWIDGET_H
