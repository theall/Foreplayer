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
#include "pluginmodel.h"

enum ColumnIndex
{
    CI_NAME=0,
    CI_MANUFACTURE,
    CI_CONTACT,
    CI_CREATE_DATE,
    CI_DESCRIPTION,
    CI_COUNT
};

TPluginModel::TPluginModel(QObject *parent) :
    QAbstractTableModel(parent)
  , mCore(NULL)
{

}

TPluginModel::~TPluginModel()
{

}

void TPluginModel::setPlayerCore(TCore *core)
{
    mCore = core;
    if(mCore)
        mPlugins = mCore->getPluginHandles();
}

QVariant TPluginModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();
        if(row>=0 && row<mPlugins.size())
        {
            PluginHandle plugin = mPlugins.at(row);
            switch (col) {
            case CI_NAME:
                return mCore->getPluginName(plugin);
                break;
            case CI_MANUFACTURE:
                return mCore->getPluginManufacture(plugin);
                break;
            case CI_CONTACT:
                return mCore->getPluginContact(plugin);
                break;
            case CI_CREATE_DATE:
                return mCore->getPluginCreateDate(plugin);
                break;
            case CI_DESCRIPTION:
                return mCore->getPluginDescription(plugin);
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}

int TPluginModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mPlugins.size();
}

int TPluginModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return CI_COUNT;
}

QVariant TPluginModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case CI_NAME:
            return tr("Name");
            break;
        case CI_MANUFACTURE:
            return tr("Manufacture");
            break;
        case CI_CONTACT:
            return tr("Contact");
            break;
        case CI_CREATE_DATE:
            return tr("Create date");
            break;
        case CI_DESCRIPTION:
            return tr("Description");
            break;
        default:
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

//// Suffix list model
TSuffixModel::TSuffixModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

TSuffixModel::~TSuffixModel()
{

}

void TSuffixModel::setSuffixDecription(QHash<QString, QString> sd)
{
    mSuffixList = sd.keys();
    mDesctiption = sd.values();

    emit layoutChanged();
}

int TSuffixModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return mSuffixList.size();
}

int TSuffixModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

QVariant TSuffixModel::data(const QModelIndex &index, int role) const
{
    if(role==Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();
        switch (col) {
        case 0:
            if(row>=0 && row<mSuffixList.size())
                return mSuffixList[row];
            break;
        case 1:
            if(row>=0 && row<mDesctiption.size())
                return mDesctiption[row];
            break;
        default:
            break;
        }
    }
    return QVariant();
}

QVariant TSuffixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return tr("Suffix");
            break;
        case 1:
            return tr("Description");
            break;
        default:
            break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
