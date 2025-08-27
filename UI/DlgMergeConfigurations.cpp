/**************************************************************************************** 
 *                                                                                      * 
 *     Folco - Program allowing to quickly change the IPv4 address of an interface      * 
 *                       Copyright (C) 2024-2025 Martial Demolins                       * 
 *                                                                                      * 
 *         This program is free software: you can redistribute it and/or modify         * 
 *         it under the terms of the GNU General Public License as published by         * 
 *          the Free Software Foundation, either version 3 of the License, or           * 
 *                          (at your option) any later version                          * 
 *                                                                                      * 
 *            This program is distributed in the hope that it will be useful            * 
 *            but WITHOUT ANY WARRANTY; without even the implied warranty of            * 
 *            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             * 
 *                     GNU General Public License for more details                      * 
 *                                                                                      * 
 *          You should have received a copy of the GNU General Public License           * 
 *         along with this program.  If not, see <https://www.gnu.org/licenses>         * 
 *                                                                                      * 
 ****************************************************************************************/

#include "DlgMergeConfigurations.hpp"
#include "../Global.hpp"
#include "../Network/ConfigurationList.hpp"
#include "../Network/PredefinedIP.hpp"
#include "ui_DlgMergeConfigurations.h"
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QNetworkInterface>
#include <QVariant>

DlgMergeConfigurations::DlgMergeConfigurations()
    : QDialog(nullptr)
    , ui(new Ui::DlgMergeConfigurations)
    , Dialog(this)
{
    ui->setupUi(this);
    setWindowTitle(APPLICATION_NAME " - Merge Configurations");

    // Create a list of tooltips at the global level because it is shared by the two lists
    QList<QString> TooltipList;

    // Fill source list
    QList<Configuration*> ConfigurationList = ConfigurationList::instance()->configurationList();

    for (int i = 0; i < ConfigurationList.size(); i++) {
        Configuration* Configuration = ConfigurationList.at(i);

        // Generate item text
        QString Text       = Configuration->humanReadableName();
        QString CustomName = Configuration->customName();
        if (!CustomName.isEmpty()) {
            Text.append(QString(" - %1").arg(CustomName));
        }

        // Create the item with its data and add it to the list
        QListWidgetItem* Item = new QListWidgetItem(Text);
        ui->ListWidgetSource->addItem(Item);
        Item->setData(CONFIGURATION_ROLE, QVariant::fromValue(Configuration));

        // Create the tooltip
        QList<PredefinedIP*> PredefinedIPlist = Configuration->predefinedIPlist();
        QString              Tooltip;
        for (int j = 0; j < PredefinedIPlist.size(); j++) {
            QString Line(PredefinedIPlist.at(j)->ipAddress()); // Line base: IP address
            QString Name(PredefinedIPlist.at(j)->name());      // Read the name
            if (!Name.isEmpty()) {                             // If a name is defined
                Line.prepend(QString("%1 - ").arg(Name));      // Add if at the beginning of the line
            }
            Tooltip.append(Line);                   // Add the line to the tooltip
            if (j != PredefinedIPlist.size() - 1) { // Add a new line if there are more IP
                Tooltip.append('\n');
            }
        }
        Item->setToolTip(Tooltip);   // Set the item tooltip
        TooltipList.append(Tooltip); // Keep a track of the tooltip for the other list
    }

    // Fill destination list
    QList<QNetworkInterface> InterfaceList = QNetworkInterface::allInterfaces();

    for (int i = 0; i < InterfaceList.size(); i++) {
        QNetworkInterface Interface     = InterfaceList.at(i);
        Configuration*    Configuration = nullptr;
        int               TooltipIndex  = -1; // Default: no configuration, so no Configuration-related tooltip

        // Try to find a matching configuration
        for (int j = 0; j < ConfigurationList.size(); j++) {
            if (Interface.hardwareAddress() == ConfigurationList.at(j)->hardwareAddress()) {
                Configuration = ConfigurationList.at(j);
                TooltipIndex  = j;
                break;
            }
        }

        // Generate item text
        QString Text = Interface.humanReadableName();
        if (Configuration != nullptr) {
            QString CustomName = Configuration->customName();
            if (!CustomName.isEmpty()) {
                Text.append(QString(" - %1").arg(CustomName));
            }
        }

        // Create the item and add it to the list
        QListWidgetItem* Item = new QListWidgetItem(Text);
        Item->setData(INTERFACE_ROLE, QVariant::fromValue(Configuration));
        ui->ListWidgetDestination->addItem(Item);

        // Add the tooltip
        if (TooltipIndex == -1) {
            Item->setToolTip("No configuration defined for this interface");
        }
        else {
            Item->setToolTip(TooltipList.at(TooltipIndex));
        }
    }

    // Connections
    connect(ui->ButtonClose, &QPushButton::clicked, this, [this]() { close(); });
    connect(ui->ListWidgetSource, &QListWidget::itemSelectionChanged, this, [this]() { updateButtons(); });
    connect(ui->ListWidgetDestination, &QListWidget::itemSelectionChanged, this, [this]() { updateButtons(); });
}

DlgMergeConfigurations::~DlgMergeConfigurations()
{
    delete ui;
}

void DlgMergeConfigurations::mergeConfigurations()
{
    DlgMergeConfigurations* Dlg = new DlgMergeConfigurations;
    Dlg->exec();
    delete Dlg;
}

void DlgMergeConfigurations::updateButtons()
{
    QList<QListWidgetItem*> SourceSelection      = ui->ListWidgetSource->selectedItems();
    QList<QListWidgetItem*> DestinationSelection = ui->ListWidgetSource->selectedItems();
    bool                    MergeEnabled         = true; // Default : both buttons enabled
    bool                    OverwriteEnabled     = true;

    // We need selections in both lists to perform an action
    if (SourceSelection.isEmpty() || DestinationSelection.isEmpty()) {
        MergeEnabled     = false;
        OverwriteEnabled = false;
    }

    // Else Merge is always enabled
    // Overwrite is only if the destination already holds a Configuration. Let's check the data embedded in the item
    else {
        QListWidgetItem* Item          = DestinationSelection.at(0);
        Configuration*   Configuration = Item->data(INTERFACE_ROLE).value<class Configuration*>();
        OverwriteEnabled               = Configuration != nullptr;
    }

    // Update buttons
    ui->ButtonMerge->setEnabled(MergeEnabled);
    ui->ButtonOverwrite->setEnabled(OverwriteEnabled);
}
