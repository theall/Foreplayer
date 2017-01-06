#include <QApplication>

#include "gui/guimanager.h"
#include "controller/maincontroller.h"
#include "core/core.h"

#include "preferences.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationDomain("Theall");
    app.setApplicationName("Foreplayer");
    app.setApplicationVersion("0.0.1");
    
    TPreferences::instance();

    TCore core;

    TMainController controller;

    TGuiManager gui(&controller);

    int result = 0;

    //if(gui.loadSkin("Z:/skins/fulkfour/skin.xml"))
    if(gui.loadSkin("z:/skins/fulkfour.zip"))
    {
        gui.showGui();

        QStringList titles;
        titles.append("13: 岁月无声 beyond IV (from 1983-2003) contributed");
        titles.append("标题: 岁月无声 beyond IV");
        titles.append("艺术家: beyond beyond IV");
        titles.append("专辑: beyond IV (from 1983-2003) contributed to beyond.Spirit of wong.");
        titles.append("格式: MP3 44KHZ 320K");


        gui.mainWindow()->setCaption("fulkfour is coming!");
        gui.mainWindow()->setTitles(titles);
        gui.mainWindow()->setTime(99, 354);

        controller.joint(&gui, &core);

        result = app.exec();
        TPreferences::deleteInstance();
    } else {

    }

    return result;
}
