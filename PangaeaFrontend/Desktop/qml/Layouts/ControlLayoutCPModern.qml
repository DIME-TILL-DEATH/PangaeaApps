import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import Qt.labs.platform 1.1 as Labs

import QtQuick.Window 2.15

import Modules 1.0
import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: mainUi
    anchors.fill: parent
    focus: true
    spacing: 2

    property bool isLA3Mode: UiCore.currentDevice.deviceType === DeviceType.LA3

    ModulesConfigWindow{
        id: _modulesConfigWindow
    }

    IrManagerWindow{
        id: _irManagerWindow
    }

    HeadCPModern
    {
        id: headCP

        width:  parent.width
        height: parent.height/1000*150

        visible: !isLA3Mode

        onOpenIrManagerWindow: {
            _irManagerWindow.show();
        }

        onOpenModulesConfigWindow: {
            _modulesConfigWindow.show();
        }
    }

    HeadLA3
    {
        id: headLA3

        width:  parent.width
        height: parent.height/1000*150

        visible: isLA3Mode

        onOpenIrManagerWindow: {
            _irManagerWindow.show();
        }

        onOpenModulesConfigWindow: {
            _modulesConfigWindow.show();
        }
    }

    ModulesListModern
    {
        id: _mainRow

        width: parent.width
        height: parent.height/1000*850
    }
}
