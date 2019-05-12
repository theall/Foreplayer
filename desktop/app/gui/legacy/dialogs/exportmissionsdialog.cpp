/*
 * Copyright (C) Bilge Theall, wazcd_1608@qq.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "exportmissionsdialog.h"
#include "ui_exportmissionsdialog.h"

#include <QPainter>
#include <QMessageBox>

#include "utils.h"

TExportItemDelegate::TExportItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void TExportItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2) {
        float value = index.model()->data(index).toFloat() * 100;
        QStyleOptionProgressBar pbOption;
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
  , mContextMenu(NULL)
  , mActionStart(NULL)
  , mActionPause(NULL)
  , mActionDelete(NULL)
  , mActionExplore(NULL)
  , mActionRestart(NULL)
{
    ui->setupUi(this);

    ui->tvMissions->setFrameShape(QFrame::Panel);
    ui->tvMissions->setFrameShadow(QFrame::Sunken);
    ui->tvMissions->setShowGrid(false);
    ui->tvMissions->verticalHeader()->setVisible(false);
    ui->tvMissions->verticalHeader()->setDefaultSectionSize(20);
    ui->tvMissions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvMissions->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tvMissions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tvMissions->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tvMissions->setMouseTracking(true);
    ui->tvMissions->setItemDelegate(new TExportItemDelegate(this));
    ui->tvMissions->horizontalHeader()->setStretchLastSection(true);
    ui->tvMissions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tvMissions->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvMissions,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(slotOnCustomizedContextMenu(QPoint)));
    connect(ui->tvMissions,
            SIGNAL(doubleClicked(QModelIndex)),
            this,
            SLOT(slotDoubleClickMission(QModelIndex)));

    createContextMenu();
    setWindowModality(Qt::NonModal);

    retranslateUi();
}

TExportMissionsDialog::~TExportMissionsDialog()
{
    delete ui;
}

void TExportMissionsDialog::setModel(QAbstractTableModel *model)
{
    ui->tvMissions->setModel(model);
    ui->tvMissions->resizeColumnsToContents();

    connect(ui->tvMissions->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,
            SLOT(slotOnSelectionChanged(QItemSelection,QItemSelection)));
}

void TExportMissionsDialog::on_btnSetting_clicked()
{
    emit requestOpenSettingDialog();
}

void TExportMissionsDialog::retranslateUi()
{
    ui->retranslateUi(this);

    mActionStart->setText(tr("Start"));
    mActionPause->setText(tr("Pause"));
    mActionDelete->setText(tr("Delete"));
    mActionExplore->setText(tr("Explore"));
    mActionRestart->setText(tr("Restart"));
}

void TExportMissionsDialog::on_btnClose_clicked()
{
    emit requestCloseWindow();

    hide();
}

void TExportMissionsDialog::on_btnStart_clicked()
{
    QList<int> rows = getSelectedRows();
    if(rows.size() < 1)
        return;

    emit requestStartMissions(rows);

    updateUI();
}

void TExportMissionsDialog::on_btnDelete_clicked()
{
    if(QMessageBox::question(
                this,
                tr("Question"),
                tr("This will cancel the selected missions, are you sure?")) != QMessageBox::Yes)
        return;

    QList<int> rows = getSelectedRows();
    if(rows.size() > 0)
    {
        emit requestRemoveMissions(rows);
        ui->tvMissions->clearSelection();
        updateUI();
    }
}

void TExportMissionsDialog::slotOnCustomizedContextMenu(QPoint pos)
{
    if(mContextMenu)
        mContextMenu->popup(pos);
}

void TExportMissionsDialog::slotDoubleClickMission(QModelIndex)
{
    updateUI();

    if(ui->btnStart->isEnabled())
        ui->btnStart->click();
    else if(ui->btnPause->isEnabled())
        ui->btnPause->click();

    updateUI();
}

QList<int> TExportMissionsDialog::getSelectedRows()
{
    QSet<int> rows;
    for(QModelIndex index : ui->tvMissions->selectionModel()->selectedRows())
        rows.insert(index.row());

//    if(rows.size()<=0)
//    {
//        int rowCount = ui->tvMissions->model()->rowCount();
//        for(int i=0;i<rowCount;i++)
//            rows.insert(i);
//    }

    return rows.toList();
}

void TExportMissionsDialog::createContextMenu()
{
    if(!mContextMenu)
    {
        mContextMenu = new QMenu(ui->tvMissions);
        mActionStart = mContextMenu->addAction(QString(), this, SLOT(slotOnActionStartTriggered()));
        mActionPause = mContextMenu->addAction(QString(), this, SLOT(slotOnActionPauseTriggered()));
        mActionDelete = mContextMenu->addAction(QString(), this, SLOT(slotOnActionDeleteTriggered()));
        mContextMenu->addSeparator();
        mActionExplore = mContextMenu->addAction(QString(), this, SLOT(slotOnActionExploreTriggered()));
        mContextMenu->addSeparator();
        mActionRestart = mContextMenu->addAction(QString(), this, SLOT(slotOnActionRestartTriggered()));
    }
}

void TExportMissionsDialog::updateUI()
{
    QList<int> rows = getSelectedRows();
    bool hasSelection = rows.size() > 0;
    ui->btnDelete->setEnabled(hasSelection);
    mActionDelete->setEnabled(hasSelection);
    mActionExplore->setEnabled(hasSelection);
    if(!hasSelection)
    {
        ui->btnStart->setEnabled(false);
        ui->btnPause->setEnabled(false);
        mActionPause->setEnabled(false);
        mActionStart->setEnabled(false);
    } else {
        QList<int> status;
        emit queryMissionsStatus(rows, status);
        bool canStart = false;
        bool canPause = false;
        bool canRestart = false;
        for(int state : status)
        {
            ExportState exportState = (ExportState)state;
            if(!canStart && (exportState&ES_PAUSED))
                canStart = true;
            if(!canPause && (exportState&ES_RUNNING || exportState&ES_READY ||exportState&ES_STARTING))
                canPause = true;
            if(!canRestart && exportState&ES_ERROR)
                canRestart = true;
        }
        ui->btnPause->setEnabled(canPause);
        ui->btnStart->setEnabled(canStart);
        mActionPause->setEnabled(canPause);
        mActionStart->setEnabled(canStart);
        mActionRestart->setEnabled(canRestart);
    }
}

void TExportMissionsDialog::slotOnActionStartTriggered()
{
    ui->btnStart->click();
}

void TExportMissionsDialog::slotOnActionPauseTriggered()
{
    ui->btnPause->click();
}

void TExportMissionsDialog::slotOnActionDeleteTriggered()
{
    ui->btnDelete->click();
}

void TExportMissionsDialog::slotOnActionExploreTriggered()
{
    QList<int> rows = getSelectedRows();
    if(rows.size() < 1)
        return;

    emit requestExploreFiles(rows);
}

void TExportMissionsDialog::slotOnActionRestartTriggered()
{
    QList<int> rows = getSelectedRows();
    if(rows.size() < 1)
        return;

    emit requestRestartMissions(rows);
}

void TExportMissionsDialog::slotOnSelectionChanged(QItemSelection, QItemSelection)
{
    ui->tvMissions->clearMask();

    updateUI();
}

void TExportMissionsDialog::on_btnPause_clicked()
{
    QList<int> rows = getSelectedRows();
    if(rows.size() < 1)
        return;

    emit requestPauseMissions(rows);

    updateUI();
}
