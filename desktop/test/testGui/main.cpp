#include <QApplication>

#include "../../gui/guimanager.h"
#include "preferences.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TPreferences::instance();
    TGuiManager gui;

    gui.loadSkin("Z:/skins/fulkfour/skin.xml");

    gui.showGui();

    QStringList titles;
    titles.append("13: 岁月无声 beyond IV (from 1983-2003) contributed");
    titles.append("标题: 岁月无声 beyond IV");
    titles.append("艺术家: beyond beyond IV");
    titles.append("专辑: beyond IV (from 1983-2003) contributed to beyond.Spirit of wong.");
    titles.append("格式: MP3 44KHZ 320K");


    gui.mainWindow()->setCaption("fulkfour is coming!");
    gui.mainWindow()->setTitles(titles);
    gui.mainWindow()->setProgress(99, 354);

    int result = a.exec();

    TPreferences::deleteInstance();

    return result;
}
