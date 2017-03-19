#ifndef TTOOLBAR_H
#define TTOOLBAR_H

#include "imagebutton.h"

#include "../../share/skin.h"

class TToolBar : public QWidget, TSkinReader
{
    Q_OBJECT

public:
    enum BUTTON
    {
        BTN_ADD,
        BTN_DELETE,
        BTN_PLAYLIST,
        BTN_SORT,
        BTN_FIND,
        BTN_EDIT,
        BTN_MODE,
        BUTTON_COUNT
    };

    TToolBar(QWidget *parent=0);
    ~TToolBar();

    void setPixmaps(QPixmap normal, QPixmap hover, QColor transparentColor);
    Qt::Alignment alignment();
    void setAlignment(QPixmap pixmap, Qt::Alignment alignment);

    void uncheckButtons();
    void retranslateUi();
    void updatePos();

signals:
    void requestToggleContexMenu(TToolBar::BUTTON id, QPoint pos, bool checked);

private slots:
    void slotButtonClicked(bool);

private:
    QSize mAlignSize;
    Qt::Alignment mAlignment;
    TImageButton *mButtons[BUTTON_COUNT];

    // TSkinReader interface
public:
    void loadFromSkin(QDomElement element, TSkin *skin) Q_DECL_OVERRIDE;
};

#endif // TTOOLBAR_H
