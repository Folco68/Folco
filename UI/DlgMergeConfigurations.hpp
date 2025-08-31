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

#ifndef DLGMERGECONFIGURATIONS_HPP
#define DLGMERGECONFIGURATIONS_HPP

#include "../Network/Configuration.hpp"
#include "Dialog.hpp"
#include <QDialog>
#include <QMetaType>
#include <QNetworkInterface>
#include <QPair>

namespace Ui {
class DlgMergeConfigurations;
}

// This structure is made to be pointed to by the data() pointer of the ListWidgetDestination items.
// For some reason, it was not possible to store a QPair pointer/reference in this field.
// This structure is needed to retrieve the Configuration (if one exists) starting from a Network Interface
// which linked with an item in the destination list.
typedef struct {
    QNetworkInterface NetworkInterface;
    Configuration*    Configuration;
} DestData;

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                               DlgMergeConfigurations                                               * 
 *                                                                                                                    * 
 * This class allows to merge Configurations, in other words to give the configuration of an interface to another one * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class DlgMergeConfigurations
    : public QDialog
    , public Dialog
{
    Q_OBJECT

  public:
    static void mergeConfigurations();
    ~DlgMergeConfigurations();

  private:
    DlgMergeConfigurations(); //
    void refreshUI();         // Called by the constructor, and after the merge / replace operations
    void updateButtons();     // Called according to the selections in the source and destination lists
    void merge();             // Merge two configurations
    void replace();           // Replace a Configuration with another one

    Ui::DlgMergeConfigurations* ui;
};

// Data type used in QVariant of the data field
Q_DECLARE_METATYPE(Configuration*)
Q_DECLARE_METATYPE(DestData*)

#define CONFIGURATION_ROLE Qt::UserRole
#define INTERFACE_ROLE     (Qt::UserRole + 1)

#endif // DLGMERGECONFIGURATIONS_HPP
