import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CppObjects
import PangaeaBackend

Column
{
    id: _mainUi
    anchors.fill: parent
    focus: true
    spacing: 2


    HeadCPModern
    {
        id: _headCP100FX

        width:  parent.width
        height: parent.height/1000*200 - _mainUi.spacing * 3

        // onOpenIrManagerWindow: {
        //     _irManagerWindow.show();
        // }
    }

    ListView{
        id: _modulesListView

        model: UiCore.currentDevice.modulesListModel;

        height: parent.height/1000*250
        width: parent.width * 0.975

        anchors.horizontalCenter: parent.horizontalCenter

        spacing: width/80

        interactive: false
        orientation: ListView.Horizontal

        currentIndex: -1

        delegate: Rectangle{
            width: _modulesListView.width/14 //- _modulesListView.spacing * (14-1)
            height: _modulesListView.height * 0.95

            color: "transparent"
            border.width: 2
            border.color: index === _modulesListView.currentIndex ? "blue" : "green"

            MouseArea{
                anchors.fill: parent

                onClicked: {
                    _moduleLoader.source = ""
                    _moduleLoader.selectedModuleInstance = moduleInstance
                    _modulesListView.currentIndex = index
                }
            }
        }

        onCurrentIndexChanged: {
            switch(_moduleLoader.selectedModuleInstance.moduleType)
            {
            case ModuleType.PR: _moduleLoader.source = "../ModulesFX/PR.qml"; break;
            case ModuleType.PA: _moduleLoader.source = "../ModulesFX/PA.qml"; break;
            }
        }
    }

    Loader{
        id: _moduleLoader

        width:  parent.width
        height: parent.height/1000*250

        property var selectedModuleInstance
    }
}
