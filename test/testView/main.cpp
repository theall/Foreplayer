#include "mainwindow.h"
#include <QApplication>
#include "playlistwidget.h"
#include "../widgets/sliderbar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TPlaylistWidget p;
    p.setBaseSize(QSize(640,480));

    TScrollBar::setPixmaps(
                QPixmap("z:/skins/fulkfour/b1.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_button.bmp"),
                QPixmap("z:/skins/fulkfour/scrollbar_thumb.bmp")
                );

    TScrollBar bar(Qt::Vertical);


    TSliderBar sbar(Qt::Horizontal);


    sbar.setPixmaps(
                QPixmap(),
                    QPixmap("z:/skins/fulkfour/tb.bmp"),
                QPixmap("z:/skins/fulkfour/eqfactor_fullrl.bmp")
                    );

    p.show();

    return a.exec();
}
