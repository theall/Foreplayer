#ifndef TTOOLBAR_H
#define TTOOLBAR_H

#include "pch.h"
#include "imagebutton.h"

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
    enum TBUTTON
    {
        BTN_ADD,
        BTN_DELETE,
        BTN_PLAYLIST,
        BTN_SORT,
        BTN_FIND,
        BTN_EDIT,
        BTN_MODE,
        TOOL_BUTTON_COUNT
    };
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TImageButton *mButtons[TOOL_BUTTON_COUNT];
};

#endif // TTOOLBAR_H
