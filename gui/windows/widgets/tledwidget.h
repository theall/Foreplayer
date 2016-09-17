#ifndef TLEDWIDGET_H
#define TLEDWIDGET_H

#include "pch.h"

class TLedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TLedWidget(QWidget *parent = 0);
    void setPixmap(QPixmap pixmap);
    void setTime(int seconds, int total);
    void setAlignment(Qt::Alignment alignment);

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

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
};

#endif // TLEDWIDGET_H
