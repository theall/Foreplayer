#ifndef TTOOLBAR_H
#define TTOOLBAR_H

#include "pch.h"
#include "timagebutton.h"

class TToolBar : public QWidget
{
    Q_OBJECT

public:
    TToolBar(QWidget *parent=0);
    ~TToolBar();

    void setPixmaps(QPixmap normal, QPixmap hover);
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);
    void retranslateUi();
    void updatePos();
private slots:
    void onButtonClicked(bool);

private:
    enum BUTTON
    {
        ADD,
        DELETE,
        PLAYLIST,
        SORT,
        FIND,
        EDIT,
        MODE,
        BUTTON_COUNT
    };
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TImageButton *mButtons[BUTTON_COUNT];
};

#endif // TTOOLBAR_H
