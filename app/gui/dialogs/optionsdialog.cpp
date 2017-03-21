#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include <QStringListModel>

#include "optionplugininfo.h"
#include "optiongeneral.h"
#include "optionabout.h"
#include "optionskin.h"

const char *g_pageTitle[PI_COUNT] = {
    "General",
    "Plugins",
    "Skin",
    "About"
};

TOptionsDialog::TOptionsDialog(QWidget *parent) :
    TAbstractDialog(parent),
    ui(new Ui::TOptionsDialog)
{
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel(this);
    QStringList pageTitles;
    for(int i=0;i<PI_COUNT;i++)
    {
        pageTitles.append(tr(g_pageTitle[i]));
    }
    model->setStringList(pageTitles);
    ui->listView->setModel(model);

    mPages.append(new TOptionGeneral);
    mPages.append(new TOptionPluginInfo);
    mPages.append(new TOptionSkin);
    mPages.append(new TOptionAbout);

    retranslateUi();
}

TOptionsDialog::~TOptionsDialog()
{
    delete ui;
}

void TOptionsDialog::openPage(PageIndex pageIndex)
{
    locatePage(pageIndex);

    exec();
}

void TOptionsDialog::locatePage(PageIndex pageIndex)
{
    for(QWidget *page : mPages)
        page->hide();

    mPages[pageIndex]->setParent(ui->pageContent);
    mPages[pageIndex]->showNormal();
}

void TOptionsDialog::retranslateUi()
{
    ui->retranslateUi(this);

}

void TOptionsDialog::on_listView_clicked(const QModelIndex &index)
{
    locatePage(PageIndex(index.row()));
}

