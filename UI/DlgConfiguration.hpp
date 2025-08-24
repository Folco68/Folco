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

#ifndef DLG_CONFIGURATION_HPP
#define DLG_CONFIGURATION_HPP

#include "../Network/Configuration.hpp"
#include "Dialog.hpp"
#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
class DlgConfiguration;
}

/********************************************************************************************************************** 
 *                                                                                                                    * 
 *                                                  DlgConfiguration                                                  * 
 *                                                                                                                    * 
 *                    This dialog uses the list of PredefinedIP of an Interface to fill its table.                    * 
 *                        Only the execDlgConfiguration really create and delete PredefinedIP.                        * 
 *                                    All other functions just modify the tables.                                     * 
 *                                                                                                                    * 
 **********************************************************************************************************************/

class DlgConfiguration
    : public QDialog
    , public Dialog
{
    Q_OBJECT

  public:
    static void execDlgConfiguration(QNetworkInterface NetworkInterface);
    static void execDlgConfiguration(Configuration* configuration);
    ~DlgConfiguration();

  private:
    explicit DlgConfiguration(QNetworkInterface NetworkInterface); // For Network Interfaces
    explicit DlgConfiguration(Configuration* configuration);       // For Configuration with disconnected Network Interfaces
    void commonInitialization(Configuration* configuration); // Don't use a delegate constructor because the class inherits QDialog, which can't be built twice
    void tableSelectionChanged();
    void newPredefinedIP();
    void editPredefinedIP();
    void deletePredefinedIP();
    void moveUp();
    void moveDown();
    void forgetConfiguration(Configuration* configuration);
    void writeContent(Configuration* configuration);

    Ui::DlgConfiguration* ui;
};

typedef enum {
    COLUMN_NAME,
    COLUMN_IP_ADDRESS,
    COLUMN_NETWORK_MASK,
    COLUMN_GATEWAY
} COLUMN_HEADER;

#endif // DLG_CONFIGURATION_HPP
