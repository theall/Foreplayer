#include "exportmissionsdialog.h"
#include "ui_exportmissionsdialog.h"

TExportItemDelegate::TExportItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void TExportItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1) {
        float value = index.model()->data(index).toFloat() * 100;
        QStyleOptionProgressBarV2 pbOption;
        pbOption.rect = option.rect.adjusted(4, 4, -4, -4);
        pbOption.minimum = 0;
        pbOption.maximum = 100;
        pbOption.textAlignment = Qt::AlignRight;
        pbOption.textVisible = true;
        pbOption.progress = value;
        pbOption.text = tr("%1%").arg(pbOption.progress);

        painter->save();
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setBrush(option.palette.highlightedText());
        }
        qApp->style()->drawControl(QStyle::CE_ProgressBar, &pbOption, painter);
        painter->restore();
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}

TExportMissionsDialog::TExportMissionsDialog(QWidget *parent) :
    TAbstractDialog(parent, true),
    ui(new Ui::TExportMissionsDialog)
{
    ui->setupUi(this);

    ui->tvMissions->setFrameShape(QFrame::Panel);
    ui->tvMissions->setFrameShadow(QFrame::Sunken);
    ui->tvMissions->setShowGrid(false);
    ui->tvMissions->verticalHeader()->setVisible(false);
    ui->tvMissions->verticalHeader()->setDefaultSectionSize(20);
    ui->tvMissions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvMissions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvMissions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tvMissions->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tvMissions->setMouseTracking(true);
    ui->tvMissions->setItemDelegate(new TExportItemDelegate(this));

    retranslateUi();
}

TExportMissionsDialog::~TExportMissionsDialog()
{
    delete ui;
}

void TExportMissionsDialog::setModel(QAbstractTableModel *model)
{
    ui->tvMissions->setModel(model);
}

void TExportMissionsDialog::on_btnSetting_clicked()
{
    hide();

    emit requestCloseWindow();
}

void TExportMissionsDialog::retranslateUi()
{
    ui->retranslateUi(this);
}
