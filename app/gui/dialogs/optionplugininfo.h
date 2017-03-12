#ifndef OPTIONPLUGININFO_H
#define OPTIONPLUGININFO_H

#include <QWidget>

namespace Ui {
class TOptionPluginInfo;
}

class TOptionPluginInfo : public QWidget
{
    Q_OBJECT

public:
    explicit TOptionPluginInfo(QWidget *parent = 0);
    ~TOptionPluginInfo();

private:
    Ui::TOptionPluginInfo *ui;
};

#endif // OPTIONPLUGININFO_H
